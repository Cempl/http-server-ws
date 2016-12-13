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
                    --&gt;
                </style>

            </head>
            <!--  <table width="400" border="1" cellspacing="0" cellpadding="2" frame="all"  rules="all" bordercolordark="white" class="doc_text" bordercolor="#666666"> -->
            <table width="100%" border="1" cellspacing="0" cellpadding="3" bgcolor="#66CC99" bordercolor="#00FF99">
                <xsl:for-each select="DATA/GROUP">

                    <tr bgcolor="#666666">
                        <td>
                            <p class="doc_title3" align="center">

                                <xsl:value-of select="@name"/>

                            </p>
                        </td>
                        <td>
                            <p class="doc_title2" align="center">
                                Standard
                            </p>
                        </td>

                        <td>
                            <p class="doc_title" align="center">DESCRIPTION</p>
                        </td>
                        <td>
                            <p class="doc_title" align="center">MESSAGE</p>
                        </td>
                        <td>
                            <p class="doc_title" align="center">COMMENTS</p>
                        </td>
                    </tr>


                    <xsl:for-each select="SQLCLASS">
                        <xsl:sort order="ascending" select="./@name"/>

                        <tr bgcolor="#999999">
                            <td>
                                <p class="doc_title" align="center">
                                    <xsl:text>&#160;</xsl:text>
                                    <xsl:value-of select="@name"/>
                                </p>
                            </td>
                            <td>
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
                            </td>

                            <td colspan="3">
                                <p class="doc_title" align="center">
                                    <xsl:text>&#160;</xsl:text>
                                    <xsl:value-of select="@description"/>
                                </p>


                            </td>

                        </tr>


                        <xsl:for-each select="SQLSTATE">
                            <xsl:sort order="ascending" select="./@name"/>

                            <tr>
                                <td>
                                    <p class="doc_text_bold">
                                        <xsl:text>&#160;</xsl:text>
                                        <xsl:value-of select="@name"/>
                                    </p>
                                </td>
                                <td>
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

                                <td>
                                    <p class="doc_text">
                                        <xsl:text>&#160;</xsl:text>
                                        <xsl:value-of select="DESCRIPTION"/>
                                    </p>


                                </td>

                                <td>
                                    <p class="doc_text_bold">
                                        <xsl:text>&#160;</xsl:text>
                                        <xsl:value-of select="MESSAGE"/>
                                    </p>

                                </td>

                                <td>
                                    <p class="doc_text">
                                        <xsl:text>&#160;</xsl:text>
                                        <xsl:value-of select="COMMENTS"/>
                                    </p>

                                </td>

                            </tr>
                        </xsl:for-each>
                    </xsl:for-each>
                </xsl:for-each>

            </table>
        </html>
    </xsl:template>

</xsl:stylesheet>
