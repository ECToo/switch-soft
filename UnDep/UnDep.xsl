<?xml version="1.0" encoding="ISO-8859-1"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
<xsl:output method="html"/>


<xsl:variable name="up" select="'ABCDEFGHIJKLMNOPQRSTUVWXYZ'"/>
<xsl:variable name="lo" select="'abcdefghijklmnopqrstuvwxyz'"/>

<xsl:template match="/">
<html>
<head><title>Package Dependencies</title><link rel="stylesheet" type="text/css" href="UnDep.css" /></head>	
<body><xsl:apply-templates/></body>
</html>
</xsl:template>

			<xsl:param name="last" select="position()"/>


<xsl:template match="bypackage">

	<div class="block">
	<br/><br/>
	
		<div class="splitter">By Package</div>		
		<div class="heading">
			<div class="pkg">Package: </div>
			<div class="file">File: </div>
			<div class="dir">Directory: </div>
		</div>
	
		<xsl:for-each select="package">		
			<xsl:sort select="name" order="ascending"/>
			<xsl:variable name="last" select="position()"/>
		
			<div class="group">		
			<xsl:for-each select="file">
				<xsl:sort select="name" order="ascending"/>
				<xsl:sort select="dir" order="ascending"/>				
				<div>
					<xsl:choose>
						<xsl:when test="$last mod 2 = 0"><xsl:attribute name="class">itemlight</xsl:attribute></xsl:when>
						<xsl:when test="$last mod 2 = 1"><xsl:attribute name="class">itemdark</xsl:attribute></xsl:when>
					</xsl:choose>	
					<div class="pkg"><xsl:value-of select="../name"/></div>
					<div class="file"><xsl:value-of select="name"/></div>
					<div class="dir"><xsl:value-of select="dir"/></div>
				</div>	  				
			</xsl:for-each>	
			</div>		
					
		</xsl:for-each>
	</div>	
<br/><br/>
</xsl:template>











<xsl:template match="byfile">

	<div class="block">
	
		<div class="splitter_file">By File</div>		
		<div class="heading_file">
			<div class="dir">Directory: </div>
			<div class="file">File: </div>
			<div class="pkg">Package: </div>
		</div>
	
		<xsl:for-each select="file">		
			<xsl:sort select="name" order="ascending"/>
			<xsl:sort select="dir" order="ascending"/>
			<xsl:variable name="last" select="position()"/>	
			
			<div class="group">			
			<xsl:for-each select="package">
				<xsl:sort select="." order="ascending"/>		
				<div>
					<xsl:choose>
						<xsl:when test="$last mod 2 = 0"><xsl:attribute name="class">itemlight</xsl:attribute></xsl:when>
						<xsl:when test="$last mod 2 = 1"><xsl:attribute name="class">itemdark</xsl:attribute></xsl:when>
					</xsl:choose>	
					<div class="dir"><xsl:value-of select="../dir"/></div>
					<div class="file"><xsl:value-of select="../name"/></div>
					<div class="pkg"><xsl:value-of select="."/></div>
				</div>	  		
			</xsl:for-each>	
			</div>	
								
		</xsl:for-each>

	</div>	
	
<br/><br/>
</xsl:template>



</xsl:stylesheet>
