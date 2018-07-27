#! /bin/sh
#  $Id: make-tool.sh,v 6.6 1999/12/06 18:31:22 kimelman Exp $
#  $RCSfile: make-tool.sh,v $

progname=$0
options="$*"

usage() {
    if [ "x$1" != x ] ; then
        echo "Unrecognized option <$1>" >&2
    fi
    cat >&2 <<EOF

    Usage: 
      $progname --help 
            output this message

      $progname --config <config_file> [<config options>...]
            create configuration file for make (included by makefile.A)
            
      $progname --install <target.install> <webdir> <mode>
            
    config options:
        --mode=MODE
                where MODE is either:
                production 
                development (default)
        --target=TARGET
                where TARGET is either:
                mmdbsrv
                vastsrv
                loader
                web     (default) include mmdbsrv & vastsrv
                all     
        --retrieval=RMODE
                where RMODE is either a combination of :
                  FF - for file based retrieval
                  PS - for Sybase based retrieval
        --threads=Y/N (N by default)
EOF
    if [ "x$1" != x ] ; then
        exit 1
    fi
    exit 0
}

emit_config() {
    mode=development
    target=all
    threads=N
    retrieval=FF
    OS=`uname -s`
    
    if   [ $OS != SunOS -a $OS != IRIX64 ] ; then
        echo "Unknown operating system($OS) - configuration failed" >&2
        exit 1
    fi

    while [ $# -gt 0 ] ; do
        case $1 in
            --mode=prod*)   mode=production  ;;
            --mode=deve*)   mode=development ;;
            --target=mmdb*) target=mmdb      ;;
            --target=vast*) target=vast      ;;
            --target=load*) target=load      ;;
            --target=web)   target=web       ;;
            --target=all)   target=all       ;;
            --threads=Y)    threads=Y        ;;
            --threads=N)    threads=N        ;;
            --retrieval=FF) retrieval=FF     ;;
            --retrieval=PS) retrieval=PS     ;; 
            *)              usage $1         ;;
        esac
        shift
    done
    
    if [ $threads = Y ] ; then
        echo 'SYBLIBRARY = $(NCBI_SYBLIBS_CT_r)'
        echo 'LDFLAGS1=$(NCBI_THR_ALTOBJ) -lthread'
    else
        echo 'SYBLIBRARY = $(NCBI_SYBLIBS_CT)'
    fi
    
    #
    # production/development mode customization
    #
    echo "mode=$mode"
    if [ $mode = production ] ; then
        echo 'OPTFLAG  = $(NCBI_OPTFLAG)'
        echo 'LIBPATH  = $(NCBI_LIBDIR)'
        if   [ $OS = SunOS  ] ; then
            echo 'webdir=/net/neptune/pubmed/WWW/test/www.server/cgi-bin/Entrez/Structure'
            echo 'loaddir=bin'
            echo 'testurl=http://neptune/cgi-bin/Entrez/Structure'
        else
            echo 'webdir=/net/vaster/usr/attic/httpd.public/cgi-bin/Structure'
            echo 'loaddir=/net/vaster/usr/people2/bryant/MDB/Pubstruct'
            echo 'testurl=http://vaster/cgi-bin/Structure'
        fi
    else
        echo 'OPTFLAG  = -g'
        echo 'LIBPATH  = $(NCBI_ALTLIB)'
        if   [ $OS = SunOS  ] ; then
            echo 'webdir=/net/neptune/pubmed/WWW/dvlp/www.server/cgi-bin/Entrez/Structure'
            echo 'loaddir=bin'
            echo 'testurl=http://neptune:5701/cgi-bin/Entrez/Structure'
        else
            echo 'webdir=/net/vaster/usr/attic/httpd.local/cgi-bin/Structure'
            echo 'loaddir=bin'
            echo 'testurl=http://vaster:6224/cgi-bin/Structure'
        fi
    fi

    echo "RMODE=${retrieval}"
    
    case $target in 
        mmdb) 
            echo "TARGET=mmdbsrv"
            ;;
        vast) 
            echo "TARGET=vastsrv"
            ;;
        load) 
            echo "TARGET=loader"
            ;;
        web)
            echo "TARGET=mmdbsrv vastsrv"
            ;;
        all)
            echo "TARGET=mmdbsrv vastsrv loader"
            ;;
    esac
}


case $1 in
    --help)
        usage 
        ;;
    --config)
        cfile=$2
        shift 2
        emit_config $* >$cfile
        ;;
    --install)
        fname=$2
        fbase=`basename $fname .FF`
        fbase=`basename $fbase .PS`
        webdir=$3
        mode=$4
        if [ x$mode = xproduction ] ; then
            ext=REAL
        else
            ext=NEW
        fi
	cp $fname ${webdir}/$fname
	cd ${webdir} 
        [ -r $fbase ] || ln -s wrapper.sh $fbase ;
        [ ! -r $fbase.$ext ] || rm -f $fbase.$ext
        ln -s $fname $fbase.$ext
	whoami >${fbase}.recepient
        ;;
    --test)
        webdir=$2
        binary=$3
        mode=$4
        if [ x$mode = xproduction ] ; then
            ext=
        else
            ext=.NEW
        fi
        cmdline="$5"
        cd $webdir
        . ./.syb_set
        SYBASE=$SYBASE_conf
        LANG=
        export SYBASE LANG
        echo "" | ./$binary$ext "$cmdline" >/dev/null
        ;;
    *)
        usage $*
        ;;
esac
exit 0
