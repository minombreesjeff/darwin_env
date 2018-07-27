var inGo=false;

function Go(cmd)
{
 inGo=true;
 var frm = document.frmQueryBox;
 var sel = frm.db;
 frm.CMD.value = escape(cmd);
 if ( cmd =='Pager' ) { frm.cmd.value = ''; }  else { frm.cmd.value = cmd; }
 var q = frm.action;
 frm.action = q.substring(0, q.indexOf('?')) + '?CMD=' +frm.CMD.value + '&DB='+sel.options[sel.selectedIndex].value;
 frm.submit();
 inGo=false;
}

function KeyPress(cmd,e)
{
 var nav = ( navigator.appName == "Netscape" ) ? true : false;
 var msie = ( navigator.appName.indexOf("Microsoft") != -1 ) ? true : false;
 var k = 0;
 if( nav ) { k = e.which; }
 else if( msie ) { k = e.keyCode; }
 if( k==13 ) Go(cmd); 
}

function DbChange(sel)
{
 // restore filter value
 if( sel.form.filters != null ) {
  var hidden = sel.options[sel.selectedIndex].value + 'Filters';
  if( sel.form.elements[hidden] != null ) { sel.form.filters.checked = true; }
  else { sel.form.filters.checked = false; }
 }
 if( sel.options[sel.selectedIndex].value != sel.form.orig_db.value &&
     sel.form.cmd_current.value != '' ) {
  sel.form.cmd.value = sel.form.cmd_current.value;
  Go(sel.form.cmd.value);
 }
}

function AddTerm(sel,op,term,field,search)
{
 var f=field[field.selectedIndex].value;
 var idx = -1;
 if(sel != null ) {idx = sel.selectedIndex; }
 if(idx < 0 && search.value!=''){
  f=(f=='All Fields')?'':'['+f+']';
  var v=search.value+f;
  search.value='';
  if(term.value=='') term.value=v; else term.value+=' '+op+' '+v;
 }
 else if(idx >= 0){
  var q=(f=='Author')?'':'"';
  var count=0;
  var vv='';
  for(i=0; i<sel.options.length; ++i) {
   if(sel.options[i].selected) {
    count++;
    vv=vv+(vv!=''?' OR ':'');
	vv=vv+q+sel.options[i].value+q+'['+f+']';
	sel.options[i].selected=false;
   }
  }
  if(count>1)vv='('+vv+')';
  if(term.value=='') term.value=vv; else term.value+=' '+op+' '+vv;
  search.value='';
 }
 term.focus();
}

function UpdateDetails(cmd)
{
 if( document.frmQueryBox.details_term != null ) {
  KillNewLines(document.frmQueryBox.details_term);
 }
 Go(cmd);
}

function ShowURL(cmd)
{
 if( document.frmQueryBox.details_term != null ) {
  var frm=document.frmQueryBox;
  KillNewLines(frm.details_term);
  var sel = frm.db;
  var pos = window.location;
  var newUrl=pos.protocol+"//"+pos.host+pos.pathname;
  newUrl+="?cmd="+cmd+"&db="+sel.options[sel.selectedIndex].value;
  newUrl+="&details_term="+escape(frm.details_term.value);
  window.location = newUrl;
 }
}

function KillNewLines(field)
{
 var spc=0,v1='',v2='',s = field.value;
 for(i=0; i<s.length; ++i) {
  if(s.charAt(i) == '\n' || s.charAt(i) == '\r') v1+=' ';
  else v1+=s.charAt(i);
 }
 for(i=0; i<v1.length; ++i) {
  if(v1.charAt(i) != ' ') {
   if(spc == 1) v2+=' ';
   spc=0; v2+=v1.charAt(i);
  }
  else if(spc == 0)
   spc=1;
 }
 field.value = v2;
}

function fmtY(y) {if(y<1900) y+=1900; return y;}
function fmtM(m) {m++; if(m<10) m='0'+m; return m;}
function fmtD(d) {if(d<10) d='0'+d; return d;}

function OnRelDateChange(field)
{
 // get field_index - we'll need it to access adjacent elements[]
 var el = document.frmQueryBox.elements;
 for(field_index=0;field_index<el.length;field_index++) {
   if(el[field_index]==field) break;
 }

 var v=field.options[field.selectedIndex].value.toLowerCase();
 if(v.length==0) {
   // 1st line (e.g. "Publication Date") selected: clear the range
   for(j=1;j<=6;j++) el[field_index+j].value="";
   return;
 }

 // convert the string like "X Days/Weeks/Months/Years" to (d,m,y) deltas
 var arr=v.split();
 var n=1; // the number of time units - could be anywhere in the string
 for(j=0;j<arr.length;j++) {
  var t=parseInt(arr[j]);
  if(t>0 && !isNaN(t)) {n=t;break;}
 }
 var y=0,m=0,d=0;
 if     (v.indexOf("week" )>=0) d=n*7;
 else if(v.indexOf("month")>=0) m=n;
 else if(v.indexOf("year" )>=0) y=n;
 else if(v.indexOf("today")< 0) d=n; // else: all remain 0, which makes "today" range

 // To = today
 var dt = new Date();
 el[field_index+4].value=fmtY(dt.getYear ());
 el[field_index+5].value=fmtM(dt.getMonth());
 el[field_index+6].value=fmtD(dt.getDate ());

 // From = today - (y,m,d)
 var msPerDay=24.0*3600.0*1000.0;
 if(m>dt.getMonth()) {y=1;m=m-12;}
 dt.setTime (      dt.getTime ()  - d*msPerDay );
 dt.setMonth(      dt.getMonth()  - m );
 dt.setYear ( fmtY(dt.getYear ()) - y ); // in 2000, Netscape's getYear() returns 100
 el[field_index+1].value=fmtY(dt.getYear ());
 el[field_index+2].value=fmtM(dt.getMonth());
 el[field_index+3].value=fmtD(dt.getDate ());
}

function ClearCheckboxes(s)
{
 var n="pmfilter_"+s;
 var el = document.frmQueryBox.elements;
 for(i=0;i<el.length;i++) {
   if(el[i].type=="checkbox" && el[i].name==n) {
     el[i].checked=false;
   }
 }
}

function Anchor(x)
{
 location=location.href.slice(0,location.href.length-location.hash.length)+'#'+x
}

function ShowLinks(url,linkscount)
{        
   var X,Y;
   var H = (linkscount + 4)*30, W = 300;
   if(parseFloat(navigator.appVersion)>= 4) { 
     if(navigator.appName=="Netscape") {
        X=window.innerWidth;Y=window.innerHeight;
        if(H > window.innerHeight) { H=window.innerHeight-50;}
     }else{
	    X=document.body.offsetWidth;Y=document.body.offsetHeight;
        if(H > document.body.offsetHeight) { H=window.innerHeight-50;}
	 }
     Y=(screen.height)/2-H/2; 
     X=(screen.width)/2-W/2; 
   }
   window.open(url, 'Links','alwaysRaised=yes,screenX='+String(X)+',screenY='+String(Y)+',resizable=no,scrollbars=yes,toolbar=no,location=no,directories=no,status=no,menubar=no,title=no,copyhistory=yes,width='+String(W)+',height='+String(H)).focus();
}

function HistViewTerm(t,op,par)
{
    if(document.frmQueryBox.term.value!='') {
        if(par==1) {
	    document.frmQueryBox.term.value='('+document.frmQueryBox.term.value+')';
	}
	if(op!='') {
    	    document.frmQueryBox.term.value=document.frmQueryBox.term.value+' '+op;
	}
    }
    document.frmQueryBox.term.value=document.frmQueryBox.term.value+' '+t;
}
