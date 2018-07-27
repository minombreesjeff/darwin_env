del asn.all
del 1.tmp 2.tmp 3.tmp

copy head.all+general.asn+biblio.asn+medline.asn+pubmed.asn+medlars.asn 1.tmp
copy 1.tmp+pub.asn+seqloc.asn+seq.asn+seqset.asn+seqcode.asn+seqblock.asn 2.tmp
copy 2.tmp+seqfeat.asn+seqalign.asn+seqres.asn+submit.asn+..\biostruc\cn3d\cn3d.asn 3.tmp
copy 3.tmp+proj.asn+access.asn+featdef.asn+ncbimime.asn+objprt.asn 4.tmp
copy 4.tmp+gbseq.asn+insdseq.asn+tinyseq.asn asn.all

del 1.tmp 2.tmp 3.tmp 4.tmp
