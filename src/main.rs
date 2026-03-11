
use axum::{
    extract::{Host, Path, ConnectInfo},
    response::IntoResponse,
    routing::get,
    Router,
};

use std::net::{SocketAddr, TcpStream};
use std::time::Duration;
use std::process::Command;

use trust_dns_resolver::TokioAsyncResolver;

use reqwest;

#[tokio::main]
async fn main() {

    let app = Router::new()
        .route("/", get(root))
        .route("/:target", get(tool_handler));

    let listener = tokio::net::TcpListener::bind("0.0.0.0:8080")
        .await
        .unwrap();

    println!("pcitools running on 8080");

    axum::serve(listener, app.into_make_service_with_connect_info::<SocketAddr>())
        .await
        .unwrap();
}

async fn root(
    Host(host): Host,
    ConnectInfo(addr): ConnectInfo<SocketAddr>,
) -> impl IntoResponse {

    let sub = host.split('.').next().unwrap_or("");

    match sub {

        "ip" => format!("{}\n", addr.ip()),

        "help" => help(),

        _ => "pcitools network api\n".to_string()
    }
}

async fn tool_handler(
    Host(host): Host,
    Path(target): Path<String>,
) -> impl IntoResponse {

    let sub = host.split('.').next().unwrap_or("");

    match sub {

        "dns" => dns_lookup(&target).await,

        "mx" => mx_lookup(&target).await,

        "ping" => run("ping",&["-c","4",&target]),

        "trace" => run("traceroute",&[&target]),

        "whois" => run("whois",&[&target]),

        "headers" => headers(&target).await,

        "geoip" => geoip(&target).await,

        "http" => http_check(&target).await,

        "ssl" => run("openssl",&["s_client","-connect",&format!("{}:443",&target)]),

        "subdomain" => subdomain(&target).await,

        "asn" => asn(&target).await,

        "portscan" => portscan(&target),

        _ => "Unknown tool\n".to_string()

    }
}

fn run(cmd:&str,args:&[&str])->String{

    match Command::new(cmd).args(args).output(){

        Ok(out)=>String::from_utf8_lossy(&out.stdout).to_string(),

        Err(e)=>format!("command error {}\n",e)

    }

}

async fn dns_lookup(target:&str)->String{

    let resolver = TokioAsyncResolver::tokio_from_system_conf().unwrap();

    match resolver.lookup_ip(target).await{

        Ok(res)=>{

            let mut out=String::new();

            for ip in res{

                out.push_str(&format!("{}\n",ip));

            }

            out
        }

        Err(e)=>format!("dns error {}\n",e)
    }

}

async fn mx_lookup(target:&str)->String{

    let resolver = TokioAsyncResolver::tokio_from_system_conf().unwrap();

    match resolver.mx_lookup(target).await{

        Ok(res)=>{

            let mut out=String::new();

            for mx in res{

                out.push_str(&format!("{}\n",mx.exchange()));

            }

            out
        }

        Err(e)=>format!("mx error {}\n",e)
    }

}

async fn headers(target:&str)->String{

    let url = format!("http://{}",target);

    match reqwest::get(&url).await{

        Ok(res)=>{

            let mut out=String::new();

            for (k,v) in res.headers(){

                out.push_str(&format!("{}: {:?}\n",k,v));

            }

            out
        }

        Err(e)=>format!("header error {}\n",e)

    }
}

async fn geoip(target:&str)->String{

    let url = format!("https://ipinfo.io/{}/json",target);

    match reqwest::get(url).await{

        Ok(r)=>r.text().await.unwrap_or("geoip error".to_string()),

        Err(e)=>format!("geoip error {}\n",e)

    }

}

async fn http_check(target:&str)->String{

    let url = format!("http://{}",target);

    match reqwest::get(url).await{

        Ok(r)=>format!("HTTP Status: {}\n",r.status()),

        Err(e)=>format!("http error {}\n",e)

    }

}

async fn subdomain(target:&str)->String{

    let url = format!("https://crt.sh/?q=%25.{}&output=json",target);

    match reqwest::get(url).await{

        Ok(r)=>r.text().await.unwrap_or("subdomain error".to_string()),

        Err(e)=>format!("subdomain error {}\n",e)

    }

}

async fn asn(target:&str)->String{

    let url = format!("https://api.hackertarget.com/aslookup/?q={}",target);

    match reqwest::get(url).await{

        Ok(r)=>r.text().await.unwrap_or("asn error".to_string()),

        Err(e)=>format!("asn error {}\n",e)

    }

}

fn portscan(target:&str)->String{

    let ports = vec![21,22,25,53,80,110,139,143,443,445,3306,3389,8080];

    let mut out=String::new();

    for p in ports{

        let addr=format!("{}:{}",target,p);

        if TcpStream::connect_timeout(
            &addr.parse().unwrap(),
            Duration::from_secs(1)
        ).is_ok(){

            out.push_str(&format!("{} open\n",p));

        }

    }

    if out.is_empty(){

        "no common ports open\n".to_string()

    }else{

        out

    }

}

fn help()->String{

"
pcitools.de CLI Tools

IP
curl ip.pcitools.de

DNS
curl dns.pcitools.de/google.com

PING
curl ping.pcitools.de/google.com

MX
curl mx.pcitools.de/google.com

TRACE
curl trace.pcitools.de/google.com

WHOIS
curl whois.pcitools.de/google.com

PORTSCAN
curl portscan.pcitools.de/google.com

HEADERS
curl headers.pcitools.de/google.com

HTTP STATUS
curl http.pcitools.de/google.com

SSL CHECK
curl ssl.pcitools.de/google.com

GEOIP
curl geoip.pcitools.de/8.8.8.8

ASN
curl asn.pcitools.de/8.8.8.8

SUBDOMAIN ENUM
curl subdomain.pcitools.de/google.com

HELP
curl help.pcitools.de
".to_string()

}
