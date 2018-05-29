<?xml version="1.0" encoding="utf-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
    <xsl:output encoding="utf-8" indent="yes" method="html"/>


    <xsl:template match="/">
        <html>
            <head>
                <META http-equiv="Content-Type" content="text/html; charset=utf-8"/>
                <style type="text/css">
                    &lt;!--
                    .doc_title {  font-family: Arial, Helvetica, sans-serif; font-size: 16px; font-weight: bold; color: #000000}
                    .doc_title2 {  font-family: Arial, Helvetica, sans-serif; font-size: 16px; font-weight: bold; color: #999999}
                    .doc_title3 {  font-family: Arial, Helvetica, sans-serif; font-size: 16px; font-weight: bold; color: #699999}
                    .doc_text_bold {  font-family: Arial, Helvetica, sans-serif; font-size: 16px; font-weight: bold; color: #000000}
                    .doc_text {  font-family: Arial, Helvetica, sans-serif; font-size: 13px; color: #000000; text-align: justify}
                    .doc_text_small {  font-family: Arial, Helvetica, sans-serif; font-size: 12px; color: #000000; text-align: justify}
                    .doc_text_bold_small {  font-family: Arial, Helvetica, sans-serif; font-size: 12px; font-weight: bold; color: #000000; text-align: justify}

                    --&gt;
                </style>

                <SCRIPT LANGUAGE="javascript">
                    <xsl:comment><![CDATA[

                                       //-------------------------------------------------------------------------------------

                                       function GetChildElem(eSrc, sTagName)
                                       {
                                         var cKids = eSrc.children;
                                         for (var i = 0; i < cKids.length; i++)
                                         {
                                           if (sTagName == cKids[i].tagName)
                                           cKids[i].style.display = ("block" == cKids[i].style.display ? "none" : "block");
                                         }
                                       }

                                       //-------------------------------------------------------------------------------------
                                       function fMouseUp()
                                       {
                                           var eSrc = window.event.srcElement;
                                           GetChildElem(eSrc.parentElement, "UL");
                                       }

                                   ]]></xsl:comment>
                </SCRIPT>

            </head>

            <body ONMOUSEUP="fMouseUp()">
                <xsl:for-each select="DATA/GROUP">

                    <UL ID="ulGroup">
                        <p class="doc_title2">
                            <IMG SRC="../XSL/PIC/DC.GIF"/>

                            <xsl:value-of select="@name"/>
                        </p>
                        <xsl:for-each select="SQLCLASS">
                            <xsl:sort order="ascending" select="./@name"/>

                            <UL ID="ulClass">
                                <p class="doc_title3">

                                    <xsl:value-of select="@name"/>
                                    <xsl:text>&#160;</xsl:text>

                                    <xsl:choose>
                                        <!-- Error inapropriate symbol in class of SQLSTATE-->
                                        <xsl:when test="translate(@name, '01234ABCDEFGH56789IJKLMNOPQSTUVWXYZ', '***********************************') != '**'">
                                            <INPUT READONLY="true" TYPE="text" VALUE="ERROR"/>
                                        </xsl:when>

                                        <!-- Standard class -->
                                        <xsl:when test="starts-with(translate(@name, '01234ABCDEFGH', '*************'), '*')">
                                            <INPUT disabled="true" CHECKED="true" TYPE="checkbox"/>
                                        </xsl:when>
                                        <!-- Non Standard class -->
                                        <xsl:when test="starts-with(translate(@name, '56789IJKLMNOPQSTUVWXYZ', '**********************'), '*')">
                                            <INPUT disabled="true" TYPE="checkbox"/>
                                        </xsl:when>
                                    </xsl:choose>

                                    <xsl:text>&#160;</xsl:text>
                                    <xsl:value-of select="@description"/>
                                </p>

                                <UL ID="ulState">
                                    <table width="100%" border="1" cellspacing="0" cellpadding="3" bgcolor="#66CC99" bordercolor="#00FF99">

                                        <xsl:for-each select="SQLSTATE">
                                            <xsl:sort order="ascending" select="./@name"/>

                                            <tr>
                                                <UL ID="ulState">

                                                    <td width="3%">
                                                        <p class="doc_text_bold_small">
                                                            <xsl:text>&#160;</xsl:text>
                                                            <xsl:value-of select="@name"/>
                                                        </p>
                                                    </td>
                                                    <td width="2%">
                                                        <xsl:choose>
                                                            <!-- Error inapropriate symbol in whole SQLSTATE-->
                                                            <xsl:when test="translate(@name, '01234ABCDEFGH56789IJKLMNOPQSTUVWXYZ', '***********************************') != '*****'">
                                                                <INPUT READONLY="true" TYPE="text" VALUE="ERROR"/>
                                                            </xsl:when>
                                                            <!-- Standard class -->
                                                            <xsl:when test="starts-with(translate(@name, '01234ABCDEFGH', '*************'), '*')">
                                                                <!-- Standard SUBclass -->
                                                                <xsl:if test="starts-with(translate(substring(@name, 3, 1), '01234ABCDEFGH', '*************'), '*')">
                                                                    <INPUT disabled="true" CHECKED="true" TYPE="checkbox"/>
                                                                </xsl:if>
                                                                <!-- Non Standard SUBclass -->
                                                                <xsl:if test="starts-with(translate(substring(@name, 3, 1), '56789IJKLMNOPQSTUVWXYZ', '**********************'), '*')">
                                                                    <INPUT disabled="true" TYPE="checkbox"/>
                                                                </xsl:if>
                                                            </xsl:when>
                                                            <!-- Non Standard class -->
                                                            <!-- Subclass May begin from any symbol -->
                                                            <xsl:when test="starts-with(translate(@name, '56789IJKLMNOPQSTUVWXYZ', '**********************'), '*')">
                                                                <INPUT disabled="true" TYPE="checkbox"/>
                                                            </xsl:when>
                                                        </xsl:choose>
                                                    </td>
                                                    <td width="25%">
                                                        <p class="doc_text_bold_small">
                                                            <xsl:text>&#160;</xsl:text>
                                                            <xsl:value-of select="DESCRIPTION"/>
                                                        </p>
                                                    </td>
                                                    <td width="20%">
                                                        <p class="doc_text_small">
                                                            <xsl:text>&#160;</xsl:text>
                                                            <xsl:value-of select="MESSAGE"/>
                                                        </p>
                                                    </td>
                                                    <td width="30%">
                                                        <p class="doc_text_small">
                                                            <xsl:text>&#160;</xsl:text>
                                                            <xsl:value-of select="COMMENTS"/>
                                                        </p>
                                                    </td>
                                                </UL>
                                            </tr>

                                        </xsl:for-each>
                                    </table>
                                </UL>
                            </UL>
                        </xsl:for-each>
                    </UL>
                </xsl:for-each>
            </body>
        </html>
    </xsl:template>

</xsl:stylesheet>
