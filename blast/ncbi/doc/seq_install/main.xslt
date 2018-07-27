<?xml version='1.0' encoding='iso-8859-1'?>
<xsl:stylesheet version='1.0'
  xmlns:xsl='http://www.w3.org/1999/XSL/Transform'>

  <xsl:output method="html"
    doctype-system="http://www.w3.org/TR/html4/loose.dtd"
    doctype-public="-//W3C//DTD HTML 4.01 Transitional//EN"
    indent="yes" />
  <xsl:variable name="nullstr" />

  <xsl:template match="/">

    <html>
      <head>
        <xsl:copy-of select="/doc/title" />

        <meta http-equiv="Content-Type"
        content="text/html; charset=iso-8859-1"></meta>
        <meta name="keywords" content="Sequin"></meta>
        <meta name="description"
        content="Sequin is a stand-alone software tool developed by the NCBI for submitting and updating entries to the GenBank, EMBL, or DDBJ sequence databases.">
        </meta>

        <link rel="stylesheet" href="ncbi_sequin.css"></link>

        <xsl:element name="link">
          <xsl:attribute name="rel">stylesheet</xsl:attribute>
          <xsl:attribute name="type">text/css</xsl:attribute>
          <xsl:attribute name="href">
            <xsl:value-of select="/doc/css"/>
          </xsl:attribute>
        </xsl:element>

        <!-- Include platform- and browser-specific stylesheet to
             override std stuff -->
        <SCRIPT LANGUAGE="JavaScript" SRC="sniff.js"></SCRIPT>

        <!-- Finally, let the page override anything it likes -->
        <xsl:value-of select="/doc/extrastyle"/>

      </head>

      <body alt="background">

        <!-- Top title bar -->
        <table border="0" cellmargin="0" cellspacing="0"
          cellpadding="0" summary="Downloading Sequin" class="TOPTITLEBAR">
          <tbody>
            <tr>
              <td width="130">
                <a href="http://www.ncbi.nlm.nih.gov">
                  <img src="left.GIF" width="130" height="45" border="0" alt="NCBI Logo"></img>
                </a>
              </td>
              <td class="TOPTITLE">
                <h1>Downloading and Installing Sequin</h1>
                <H2><xsl:value-of select="/doc/title"/></H2>
              </td>
            </tr>
          </tbody>
        </table>

        <!--  the quicklinks bar   -->
        <table class="TEXT" border="0" width="600" cellspacing="0"
        cellpadding="3" bgcolor="#000000"
        summary="Links to several NCBI services">
          <tbody>
            <tr class="TEXT" align="center">
              <td width="100">
                <a href="index.html" class="BAR">Sequin</a>
              </td>
              <td width="100">
                <a href="http://www.ncbi.nlm.nih.gov/Entrez/"
                class="BAR">Entrez</a>
              </td>
              <td width="100">
                <a href="http://www.ncbi.nlm.nih.gov/BLAST/" class="BAR">
                  BLAST
                </a>
              </td>
              <td width="100">
                <a href="http://www.ncbi.nlm.nih.gov/entrez/query.fcgi?db=OMIM" class="BAR">
                  OMIM
                </a>
              </td>
              <td width="100">
                <a href="http://www.ncbi.nlm.nih.gov/Taxonomy/taxonomyhome.html"
                   class="BAR">Taxonomy</a>
              </td>
              <td width="100">
                <a href="http://www.ncbi.nlm.nih.gov/Structure/"
                class="BAR">Structure</a>
              </td>
            </tr>
          </tbody>
        </table>

        <img src="spacer10.GIF" alt="spacer" width="100%" height="15"></img><br></br>

        <!-- Main body -->
        <table border="0" width="600" cellspacing="0" cellpadding="0">
          <tbody>
            <tr valign="top">
              <xsl:choose>

                <!-- If left_menu is "no", then just insert content -->
                <xsl:when test="/doc/@left_menu = 'no'">
                  <td colspan="2" width="600">
                    <xsl:copy-of select="/doc/contents/*"/>
                  </td>
                </xsl:when>

                <!-- If left_menu is NOT no, then create left menu -->
                <xsl:otherwise>

                  <!--  left column   -->
                  <td width="145">
                    <!-- Left menu -->
                    <xsl:call-template name="left_menu"/>
                  </td>
                    
                  <!-- right content column  -->
                  <td width="455">
                    <xsl:copy-of select="/doc/contents/*"/>
                  </td>
                </xsl:otherwise>
              </xsl:choose>
              
            </tr>
          </tbody>
        </table>

        <!-- Page bottom -->
        <table border="0" cellspacing="0" cellpadding="0">
          <tr>
            <td width="145"></td>
            <td width="455">
              
              <hr style="width: 100%; height: 2px;"></hr>
              
              <div style="text-align: center;">
                <b>Questions or Comments?</b>
                <p class="medium1">Write to the 
                <a href="mailto:info@ncbi.nlm.nih.gov">
                  NCBI Service Desk
                </a>
              </p>
              <p class="medium1">Revised Feb 27, 2004</p>
            </div>
          </td>
        </tr>
      </table>
      <br></br>
      <!--  end of content  -->
    </body>
  </html>
</xsl:template>

<xsl:template name="left_menu">
  <p>
    <a href="http://ncbi.nlm.nih.gov/Sequin/index.html"
      class="GUTTER1">Sequin home</a>
    <br></br>
  </p>
  <p>
    <a href="http://ncbi.nlm.nih.gov/Sequin/QuickGuide/sequin.htm"
      class="GUTTER1">Quick Guide</a>
    <br></br>
  </p>
  <p>
    <a href="http://ncbi.nlm.nih.gov/Sequin/faq.html"
      class="GUTTER1">FAQs</a>
    <br></br>
  </p>
  <p>
    <a href="http://ncbi.nlm.nih.gov/Sequin/sequin.hlp.html"
      class="GUTTER1">Help Documentation</a>
    <br></br>
  </p>
  
  <p>
    <a href="seq_download.html" class="GUTTER1">
      Download Sequin</a>
      <br></br>
      
      <div style="margin-left: 10px; margin-top: 10px;">
        <a class="GUTTER2" href="seq_win_download.html" title="Download instructions for Windows">
          <div style="height: 10px">
            Windows
          </div>
        </a>
        <br></br>
        <a class="GUTTER2" href="seq_mac_download.html" title="Download instructions for Macintosh">
          <div style="height: 10px">
            Macintosh
          </div>
        </a>
        <br></br>
        <a class="GUTTER2" href="seq_unix_download.html" title="Download instructions for Unix">
          <div style="height: 10px">
            Unix
          </div>
        </a>
        <br></br>
        <a class="GUTTER2" href="seq_ftp_download.html" title="Download instructions for all platforms using ftp">
          <div style="height: 15px">
            Download using ftp
          </div>
        </a>
        <br></br>
        <a class="GUTTER2" href="seq_trouble.html" title="Get help with problems">
          <div style="height: 10px">
            Troubleshooting
          </div>
        </a>
      </div>
    </p>
    
  </xsl:template>
  
</xsl:stylesheet>

