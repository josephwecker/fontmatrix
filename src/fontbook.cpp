//
// C++ Implementation: fontbook
//
// Description:
//
//
// Author: Pierre Marchand <pierremarc@oep-h.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "fontbook.h"
#include "fontbookdialog.h"
#include "typotek.h"
#include "fontitem.h"

#include <QDebug>
#include <QObject>
#include <QProgressDialog>
#include <QSvgRenderer>
#include <QGraphicsSvgItem>

FontBook::FontBook()
{
	mapPSize[ "A0" ] = QPrinter::A0 ;
	mapPSize[ "A1" ] = QPrinter::A1 ;
	mapPSize[ "A2" ] = QPrinter::A2 ;
	mapPSize[ "A3" ] = QPrinter::A3 ;
	mapPSize[ "A4" ] = QPrinter::A4 ;
	mapPSize[ "A5" ] = QPrinter::A5 ;
	mapPSize[ "A6" ] = QPrinter::A6 ;
	mapPSize[ "A7" ] = QPrinter::A7 ;
	mapPSize[ "A8" ] = QPrinter::A8 ;
	mapPSize[ "A9" ] = QPrinter::A9 ;
	mapPSize[ "B0" ] = QPrinter::B0 ;
	mapPSize[ "B1" ] = QPrinter::B1 ;
	mapPSize[ "B10" ] = QPrinter::B10 ;
	mapPSize[ "B2" ] = QPrinter::B2 ;
	mapPSize[ "B3" ] = QPrinter::B3 ;
	mapPSize[ "B4" ] = QPrinter::B4 ;
	mapPSize[ "B5" ] = QPrinter::B5 ;
	mapPSize[ "B6" ] = QPrinter::B6 ;
	mapPSize[ "B7" ] = QPrinter::B7 ;
	mapPSize[ "B8" ] = QPrinter::B8 ;
	mapPSize[ "B9" ] = QPrinter::B9 ;
	mapPSize[ "Letter" ] = QPrinter::Letter ;
	mapPSize[ "Tabloid" ] = QPrinter::Tabloid ;
	mapPSize[ "Custom" ] = QPrinter::Custom ;


}


FontBook::~FontBook()
{
}




void FontBook::doBook()
{
	FontBookDialog bookOption ( typotek::getInstance() );
	bookOption.exec();

	if ( !bookOption.isOk )
		return;

	if ( bookOption.getFileName().isEmpty() )
		return;

	outputFilePath = bookOption.getFileName();

	if ( bookOption.isTemplate() )
	{
		doBookFromTemplate ( bookOption.getTemplate() );
		return;
	}
	/*
		double pageHeight = bookOption.getPageSize().height();
		double pageWidth = bookOption.getPageSize().width();
		QString theFile = bookOption.getFileName();
		double familySize = bookOption.getFontSize ( "family" );
		double headSize = bookOption.getFontSize ( "headline" );
		double bodySize = bookOption.getFontSize ( "body" );
		double styleSize = bookOption.getFontSize ( "style" );
		double familynameTab = bookOption.getTabFamily();
		double variantnameTab = bookOption.getTabStyle();
		double sampletextTab = bookOption.getTabSampleText();
		double topMargin =  bookOption.getPageSize().height() / 10.0;
		QStringList loremlist = bookOption.getSampleText().split ( '\n' );
		QString headline = bookOption.getSampleHeadline();
		QPrinter::PageSize printedPageSize = bookOption.getPageSizeConstant();
		double parSize = familySize * 3.0 + styleSize * 1.2 + headSize * 1.2 + static_cast<double> ( loremlist.count() ) * bodySize * 1.2;


		QPrinter thePrinter ( QPrinter::HighResolution );
		thePrinter.setOutputFormat ( QPrinter::PdfFormat );
		thePrinter.setOutputFileName ( theFile );
		thePrinter.setPageSize ( printedPageSize );
		thePrinter.setFullPage ( true );
		QGraphicsScene theScene;
		QRectF pageRect ( 0,0,pageWidth,  pageHeight );
		theScene.setSceneRect ( pageRect );
		QPainter thePainter ( &thePrinter );


		QString styleString ( QString ( "color:white;background-color:black;font-family:Helvetica;font-size:%1pt" ).arg ( familySize ) );

		QFont theFont;// the font for all that is not collected fonts
		theFont.setPointSize ( familySize );
		theFont.setFamily ( "Helvetica" );
		theFont.setBold ( true );

		QFont myLittleFont ( theFont );
		myLittleFont.setPointSize ( 10 );
		myLittleFont.setBold ( false );
		myLittleFont.setItalic ( true );


		QPen abigwhitepen;
		abigwhitepen.setWidth ( 10 );
		abigwhitepen.setColor ( Qt::white );


		QList<FontItem*> localFontMap = typotek::getInstance()->getCurrentFonts();
		QMap<QString, QList<FontItem*> > keyList;
		for ( int i=0; i < localFontMap.count();++i )
		{
			keyList[localFontMap[i]->value ( "family" ) ].append ( localFontMap[i] );
	// 		qDebug() << localFontMap[i]->value ( "family" ) ;
		}

		QMap<QString, QList<FontItem*> >::const_iterator kit;
		QProgressDialog progress ( QObject::tr ( "Creating font book... " ), QObject::tr ( "cancel" ), 0, keyList.count(), typotek::getInstance() );
		progress.setWindowModality ( Qt::WindowModal );
		int progressindex=0;

		QList<FontItem*> renderedFont;
		QPointF pen ( 0,0 );
		QGraphicsTextItem *title;
		QGraphicsTextItem *folio;
		QGraphicsTextItem *ABC;
		QGraphicsTextItem *teteChap;
		QGraphicsRectItem *titleCartouche;
		QGraphicsRectItem *edgeCartouche;
		QString firstLetter;
		QString pageNumStr;
		int pageNumber = 0;


		bool firstKey = true;
		for ( kit = keyList.begin(); kit != keyList.end(); ++kit )
		{
	// 		qDebug() << "\t" << kit.key();
			progress.setLabelText ( kit.key() );
			progress.setValue ( ++progressindex );
			if ( progress.wasCanceled() )
				break;

			pen.rx() = familynameTab;
			pen.ry() += topMargin;

			firstLetter.clear();
	// 		firstLetter.append ( kit.key().at ( 0 ).toUpper() );
			firstLetter.append ( kit.key().toLower() );

			if ( firstKey )
			{
				pageNumStr.setNum ( 1 );
				folio = theScene.addText ( pageNumStr,theFont );
				folio->setPos ( pageWidth * 0.9, pageHeight * 0.9 );
				folio->setZValue ( 9999000.0 );
				ABC = theScene.addText ( firstLetter.at ( 0 ).toUpper() ,theFont );
				ABC->setPos ( pageWidth *0.9,pageHeight * 0.05 );
	// 			ABC->rotate(90);
				edgeCartouche = theScene.addRect ( QRectF ( QPointF ( pageWidth * 0.85 , 0.0 ),  QSizeF ( pageWidth * 0.15, pageHeight ) ) ,abigwhitepen, Qt::lightGray );

				edgeCartouche->setZValue ( 101.0 );

				ABC->setZValue ( 10000.0 );
				ABC->setDefaultTextColor ( Qt::black );
				firstKey = false;
			}

			if ( ( pen.y() + parSize ) > pageHeight * 0.9 )
			{
				pageNumStr.setNum ( ++pageNumber );
				folio = theScene.addText ( pageNumStr,theFont );
				folio->setPos ( pageWidth * 0.9, pageHeight * 0.9 );
				folio->setZValue ( 9999000.0 );
				folio->setDefaultTextColor ( Qt::black );

				theScene.render ( &thePainter );

				thePrinter.newPage();
				pen.ry() = topMargin;
				for ( int  n = 0; n < renderedFont.count(); ++n )
				{
					renderedFont[n]->deRenderAll();

				}
				renderedFont.clear();
				theScene.removeItem ( theScene.createItemGroup ( theScene.items() ) );
				ABC = theScene.addText ( firstLetter.at ( 0 ).toUpper() ,theFont );
				ABC->setPos ( pageWidth *0.9,pageHeight * 0.05 );
	// 			ABC->rotate(90);
	// 			edgeCartouche = theScene.addRect ( pageWidth * 0.85 + 10.0 , 0.0 - 10.0,  pageWidth * 0.15, pageHeight + 20.0 ,abigwhitepen, Qt::lightGray );
				edgeCartouche = theScene.addRect ( QRectF ( QPointF ( pageWidth * 0.85 , 0.0 ),  QSizeF ( pageWidth * 0.15, pageHeight ) ) ,abigwhitepen, Qt::lightGray );
				edgeCartouche->setZValue ( 101.0 );

				ABC->setZValue ( 10000.0 );
				ABC->setDefaultTextColor ( Qt::black );

			}

			title = theScene.addText ( QString ( "%1" ).arg ( kit.key().toUpper() ), theFont );
			title->setPos ( pen );
			title->setDefaultTextColor ( Qt::white );
			title->setZValue ( 100.0 );
			QRectF cartrect ( 0,pen.y(),title->sceneBoundingRect().right(), title->sceneBoundingRect().height() );
			titleCartouche = theScene.addRect ( cartrect,QPen ( Qt::transparent ) ,Qt::black );
	// 		titleCartouche->setPos(pen);
			pen.ry() += 4.0  * familySize;

			for ( int  n = 0; n < kit.value().count(); ++n )
			{
	// 			qDebug() << "\t\t" << kit.value()[n]->variant();

				if ( ( pen.y() + ( parSize - 4.0 * familySize ) ) > pageHeight * 0.9 )
				{
					pageNumStr.setNum ( ++pageNumber );
					folio = theScene.addText ( pageNumStr,theFont );
					folio->setPos ( pageWidth * 0.9, pageHeight * 0.9 );
					folio->setDefaultTextColor ( Qt::black );
					folio->setZValue ( 1000.0 );

					theScene.render ( &thePainter );

					thePrinter.newPage();
					pen.ry() = topMargin;
					for ( int  d = 0; d <  renderedFont.count() ; ++d )
					{
						renderedFont[d]->deRenderAll();

					}
					renderedFont.clear();
					theScene.removeItem ( theScene.createItemGroup ( theScene.items() ) );
					ABC = theScene.addText ( firstLetter.at ( 0 ).toUpper() ,theFont );
					ABC->setPos ( pageWidth *0.9,pageHeight * 0.05 );
	// 				ABC->rotate(90);

					teteChap = theScene.addText ( firstLetter, myLittleFont );
					teteChap->setPos ( pageWidth * 0.66, pageHeight * 0.03 );
					teteChap->setDefaultTextColor ( Qt::gray );


	// 				edgeCartouche = theScene.addRect ( pageWidth * 0.85 + 10.0 , 0.0 - 10.0,  pageWidth * 0.15, pageHeight + 20.0 ,abigwhitepen, Qt::lightGray );
					edgeCartouche = theScene.addRect ( QRectF ( QPointF ( pageWidth * 0.85 , 0.0 ),  QSizeF ( pageWidth * 0.15, pageHeight ) ) ,abigwhitepen, Qt::lightGray );
					edgeCartouche->setZValue ( 101.0 );

					ABC->setZValue ( 10000.0 );
					ABC->setDefaultTextColor ( Qt::black );
				}
				pen.rx() =variantnameTab;
				FontItem* curfi = kit.value() [n];
	// 			qDebug() << "\tRENDER" << kit.key() << curfi->variant();
				renderedFont.append ( curfi );
				bool oldRast = curfi->rasterFreetype();
				curfi->setFTRaster ( false );
				curfi->setRTL ( false );
				curfi->renderLine ( &theScene,curfi->variant(), pen ,styleSize );

				pen.rx() = sampletextTab;
				pen.ry() +=  2.0 * styleSize;
				curfi->renderLine ( &theScene, headline,pen, headSize );
				pen.ry() +=  headSize * 0.5;
				for ( int l=0; l < loremlist.count(); ++l )
				{
					curfi->renderLine ( &theScene, loremlist[l],pen, bodySize );
					pen.ry() +=  bodySize * 1.2;
				}
				curfi->setFTRaster ( oldRast );
				pen.ry() +=styleSize * 2.0;

			}
		}
		if ( renderedFont.count() )
		{
			theScene.render ( &thePainter );
			for ( int  d = 0; d <  renderedFont.count() ; ++d )
			{
				renderedFont[d]->deRenderAll();

			}

		}*/
}

void FontBook::doBookFromTemplate ( const QDomDocument &aTemplate )
{
	

	/**
	We build lists of contexts
	*/
	QList<FontBookContext> conPage;
	QList<FontBookContext> conFamily;
	QList<FontBookContext> conSubfamily;

	QDomNodeList conList = aTemplate.elementsByTagName ( "context" );
	if ( conList.length() == 0 )
	{
		qDebug ( ) << "ERROR: "<< conList.length() <<" context in template, see yourself :" ;
		qDebug() << aTemplate.toString ( 1 );
		return;
	}

	QMap<QString, QSvgRenderer*> svgRendered;
	QMap<QString, QFont> qfontCache; // abit of optim.
	for ( uint i = 0; i < conList.length(); ++i )
	{
		FontBookContext fbc;
		QDomNode context = conList.item ( i );
		QString levelString = context.toElement().attributeNode ( "level" ).value();

		fbc.textElement.e = context.namedItem ( "text" ).toElement().text();
		if(!fbc.textElement.e.isEmpty())
		{
	// 		QString textInternalString = context.namedItem ( "text" ).toElement().attributeNode ( "internal" ).value();
	// 		fbc.textElement.internal = ( textInternalString == "true" ) ? true : false;
			fbc.textElement.valid = true;
					
			QDomNode tStyle =  context.namedItem ( "textstyle" );
			fbc.textStyle.name = tStyle.toElement().attributeNode ( "name" ).value();
			fbc.textStyle.font = tStyle.namedItem ( "font" ).toElement().text();
			fbc.textStyle.fontsize = QString ( tStyle.namedItem ( "fontsize" ).toElement().text() ).toDouble() ;
			fbc.textStyle.color = QColor( tStyle.namedItem ( "color" ).toElement().text() );
			qfontCache[fbc.textStyle.name] = QFont ( fbc.textStyle.font,fbc.textStyle.fontsize );
	
			fbc.textStyle.lineheight = QString ( tStyle.namedItem ( "lineheight" ).toElement().text() ).toDouble() ;
			fbc.textStyle.margin_top = QString ( tStyle.namedItem ( "margintop" ).toElement().text() ).toDouble() ;
			fbc.textStyle.margin_left = QString ( tStyle.namedItem ( "marginleft" ).toElement().text() ).toDouble() ;
			fbc.textStyle.margin_bottom = QString ( tStyle.namedItem ( "marginbottom" ).toElement().text() ).toDouble() ;
			fbc.textStyle.margin_right = QString ( tStyle.namedItem ( "marginright" ).toElement().text() ).toDouble() ;
		}
		
		QDomNode graphicNode = context.namedItem ( "graphic" );
		if(graphicNode.isElement())
		{
// 			QDomDocumentFragment svgFrag = aTemplate.createDocumentFragment();
			QDomNode svgNode = graphicNode.toElement().namedItem("svg").cloneNode(true);
// 			svgFrag.appendChild(svgNode);
// 			if(svgNode.isElement())
			{
				fbc.graphic.name =  graphicNode.toElement().attributeNode("name").value();
				fbc.graphic.x = QString ( graphicNode.toElement().attributeNode("xpos").value()).toDouble();
				fbc.graphic.y = QString ( graphicNode.toElement().attributeNode("ypos").value()).toDouble();
				
				QDomDocument svgDoc;
				QDomNode svg = svgDoc.importNode(svgNode,true);
				svgDoc.appendChild(svg);
				QString svgString("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>\n" + svgDoc.toString(0));
				QSvgRenderer *doc = new QSvgRenderer( svgString.toUtf8());
				svgRendered[fbc.graphic.name] = doc;
				fbc.graphic.valid = true;
// 				qDebug() << fbc.graphic.svg;
			}
			
		}
		

		if ( levelString == "page" )
			conPage << fbc;
		else if ( levelString == "family" )
			conFamily << fbc;
		else if ( levelString == "subfamily" )
			conSubfamily << fbc;
	}
	
	QPrinter thePrinter ( QPrinter::HighResolution );
	QString paperSize = QString (aTemplate.documentElement().namedItem( "papersize" ).toElement().text()).toUpper();
	thePrinter.setOutputFormat ( QPrinter::PdfFormat );
	thePrinter.setCreator("Fontmatrix " + QString::number(FONTMATRIX_VERSION_MAJOR) + "." + QString::number(FONTMATRIX_VERSION_MINOR));
	thePrinter.setDocName("A font book");
	thePrinter.setOutputFileName ( outputFilePath );
	thePrinter.setPageSize ( mapPSize[paperSize] );
	thePrinter.setFullPage ( true );
	qDebug() << thePrinter.pageSize() << thePrinter.pageRect() << thePrinter.paperRect() << thePrinter.resolution() ;
	double paperWidth  = thePrinter.pageRect().width() / thePrinter.resolution() * 72;
	double paperHeight = thePrinter.pageRect().height() / thePrinter.resolution() * 72;
	qDebug()<< paperSize << paperWidth << paperHeight;
	QGraphicsScene theScene;
	theScene.setSceneRect ( 0,0,paperWidth,paperHeight );
	QPainter thePainter ( &thePrinter );
	QPointF thePos ( 0,0 );
	QList<FontItem*> renderedFont;
	QList<QGraphicsSvgItem*> renderedGraphic;



	QList<FontItem*> localFontMap = typotek::getInstance()->getCurrentFonts();
	QMap<QString, QList<FontItem*> > keyList;
	for ( int i=0; i < localFontMap.count();++i )
	{
		keyList[localFontMap[i]->value ( "family" ) ].append ( localFontMap[i] );
	}

	QMap<QString, QList<FontItem*> >::const_iterator kit;
	QProgressDialog progress ( QObject::tr ( "Creating font book... " ), QObject::tr ( "cancel" ), 0, keyList.count(), typotek::getInstance() );
	progress.setWindowModality ( Qt::WindowModal );
	int progressindex=0;


	QString pageNumStr;
	int pageNumber = 0;
	///We begin in a PAGE context
// 	qDebug() << "PAGE";
	pageNumStr.setNum ( ++pageNumber );
	QMap<QString,QString> pageReplace;
	QMap<QString,QString> familyReplace;
	QMap<QString,QString> subfamilyReplace;
	
	QString currentFamily;
	QString currentSubfamily;
	
	/** Z policy is
		PAGE_SVG 1
		PAGE_TEXT 10
		FAMILY_SVG 100
		FAMILY_TEXT 1000
		SUBFAMILY_SVG 10000
		SUBFAMILY_TEXT 100000
	*/

	for ( int pIndex = 0; pIndex < conPage.count(); ++pIndex )
	{
		if(conPage[pIndex].textElement.valid)
		{
			QStringList pagelines ;
			QStringList tmplines = conPage[pIndex].textElement.e.split ( "\n" );
			pageReplace["##PAGENUMBER##"] = pageNumStr;
			pageReplace["##FAMILY##"] = currentFamily;
			pageReplace["##SUBFAMILY##"] = currentSubfamily;
			for ( int t = 0; t < tmplines.count(); ++t )
			{
	
				QString place = tmplines[t];
				for ( QMap<QString,QString>::const_iterator repIt = pageReplace.begin(); repIt != pageReplace.end();   ++repIt )
					place.replace ( repIt.key(),repIt.value(),Qt::CaseSensitive );
				pagelines << place;
			}
	// 			QFont pFont ( conPage[pIndex].textStyle.font, conPage[pIndex].textStyle.fontsize );
			for ( int pl = 0; pl < pagelines.count(); ++pl )
			{
				QGraphicsTextItem * ti = theScene.addText ( pagelines[pl], qfontCache[conPage[pIndex].textStyle.name] );
				ti->setPos ( conPage[pIndex].textStyle.margin_left, conPage[pIndex].textStyle.margin_top + ( pl * conPage[pIndex].textStyle.lineheight ) );
				ti->setZValue(10);
				ti->setDefaultTextColor ( conPage[pIndex].textStyle.color );
			}
		}
		if(conPage[pIndex].graphic.valid)
		{
				QGraphicsSvgItem *svgIt = new QGraphicsSvgItem();
				svgIt->setSharedRenderer(svgRendered[conPage[pIndex].graphic.name]);
				theScene.addItem(svgIt);
				svgIt->setPos(conPage[pIndex].graphic.x, conPage[pIndex].graphic.y);
				renderedGraphic << svgIt;
				svgIt->setZValue(1);
		}
	}

	/// Beginning of the big loop
	for ( kit = keyList.begin(); kit != keyList.end(); ++kit )
	{
		/// We are in a FAMILY context
// 		qDebug() << "FAMILY";
		{
			if ( progress.wasCanceled() )
				break;
			progress.setLabelText ( kit.key() );
			progress.setValue ( ++progressindex );
		}
		currentFamily = kit.key();
		for ( int elemIndex = 0; elemIndex < conFamily.count() ; ++elemIndex )
		{
			QStringList familylines;
			QStringList tmplines = conFamily[elemIndex].textElement.e.split ( "\n" );
			familyReplace["##FAMILY##"] = kit.key();
			for ( int t = 0; t < tmplines.count(); ++t )
			{

				QString place = tmplines[t];
				for ( QMap<QString,QString>::const_iterator repIt = familyReplace.begin(); repIt != familyReplace.end();   ++repIt )
					place.replace ( repIt.key(),repIt.value(),Qt::CaseSensitive );
				familylines << place;
			}

			double available = paperHeight - thePos.y();
			double needed = ( familylines.count() * conFamily[elemIndex].textStyle.lineheight )
			                + conFamily[elemIndex].textStyle.margin_top
			                + conFamily[elemIndex].textStyle.margin_bottom;

			if ( needed > available )
			{
				/// We are in a PAGE context
// 				qDebug() << "NFPAGE";
				// close, clean and create
				theScene.render ( &thePainter );

				thePos.ry() = 0;
				for ( int  d = 0; d <  renderedFont.count() ; ++d )
					renderedFont[d]->deRenderAll();
				for ( int  d = 0; d < renderedGraphic.count(); ++d)
					delete renderedGraphic[d];
				renderedFont.clear();
				renderedGraphic.clear();
				theScene.removeItem ( theScene.createItemGroup ( theScene.items() ) );

				thePrinter.newPage();
				pageNumStr.setNum ( ++pageNumber );

				//
				for ( int pIndex = 0; pIndex < conPage.count(); ++pIndex )
				{
					QStringList pagelines ;
					QStringList tmplines = conPage[pIndex].textElement.e.split ( "\n" );
					pageReplace["##PAGENUMBER##"] = pageNumStr;
					pageReplace["##FAMILY##"] = currentFamily;
					pageReplace["##SUBFAMILY##"] = currentSubfamily;
					for ( int t = 0; t < tmplines.count(); ++t )
					{

						QString pageplace = tmplines[t];
						for ( QMap<QString,QString>::const_iterator repIt = pageReplace.begin(); repIt != pageReplace.end();   ++repIt )
							pageplace.replace ( repIt.key(),repIt.value(),Qt::CaseSensitive );
						pagelines << pageplace;
					}
// 						QFont pFont ( conPage[pIndex].textStyle.font, conPage[pIndex].textStyle.fontsize );
					for ( int pl = 0; pl < pagelines.count(); ++pl )
					{
						QGraphicsTextItem * ti = theScene.addText ( pagelines[pl], qfontCache[conPage[pIndex].textStyle.name] );
						ti->setPos ( conPage[pIndex].textStyle.margin_left, conPage[pIndex].textStyle.margin_top + ( pl * conPage[pIndex].textStyle.lineheight ) );
						ti->setZValue(10);
						ti->setDefaultTextColor( conPage[pIndex].textStyle.color);
					}
					if(conPage[pIndex].graphic.valid)
					{
						QGraphicsSvgItem *svgIt = new QGraphicsSvgItem();
						svgIt->setSharedRenderer(svgRendered[conPage[pIndex].graphic.name]);
						theScene.addItem(svgIt);
						svgIt->setPos(conPage[pIndex].graphic.x, conPage[pIndex].graphic.y);
						renderedGraphic << svgIt;
						svgIt->setZValue(1);
					}
				}
			}

// 				QFont aFont ( conFamily[elemIndex].textStyle.font,conFamily[elemIndex].textStyle.fontsize );
			for ( int fl = 0; fl < familylines.count(); ++fl )
			{
				QGraphicsTextItem * ti = theScene.addText ( familylines[fl], qfontCache[conFamily[elemIndex].textStyle.name] );
				ti->setPos ( conFamily[elemIndex].textStyle.margin_left, thePos.y() +
				             ( conFamily[elemIndex].textStyle.margin_top + ( fl * conFamily[elemIndex].textStyle.lineheight ) ) );
				ti->setZValue(1000);
				ti->setDefaultTextColor(conFamily[elemIndex].textStyle.color);
			}
			if(conFamily[elemIndex].graphic.valid)
			{
				QGraphicsSvgItem *svgIt = new QGraphicsSvgItem();
				svgIt->setSharedRenderer(svgRendered[conFamily[elemIndex].graphic.name]);
				theScene.addItem(svgIt);
				svgIt->setPos(conFamily[elemIndex].graphic.x, conFamily[elemIndex].graphic.y + thePos.y());
				renderedGraphic << svgIt;
				svgIt->setZValue(100);
			}

			thePos.ry() += needed;
		} // end of FAMILY level elements

		for ( int fontIndex = 0;fontIndex < kit.value().count(); ++fontIndex )
		{
// 			qDebug() << fontIndex << "/" << kit.value().count();
			FontItem * theFont = kit.value() [fontIndex];
			bool oldRast = theFont->rasterFreetype();
			theFont->setFTRaster ( false );
			theFont->setRTL ( false );
			/// We are in a SUBFAMILY context
			currentSubfamily = theFont->variant();
			for ( int elemIndex = 0; elemIndex < conSubfamily.count() ; ++elemIndex )
			{
				// First, is there enough room for this element
				QStringList sublines;
				QStringList tmplines = conSubfamily[elemIndex].textElement.e.split ( "\n" );
// 				qDebug() <<"A";
				subfamilyReplace["##FAMILY##"] = theFont->family();
				subfamilyReplace["##SUBFAMILY##"] = theFont->variant();
				subfamilyReplace["##FILE##"]= theFont->path();
				subfamilyReplace["##TAGS##"]= theFont->tags().join(";") ; 
				subfamilyReplace["##COUNT##"]= QString::number(theFont->glyphsCount());
				subfamilyReplace["##TYPE##"]= theFont->type();
				subfamilyReplace["##CHARSETS##"]=theFont->charmaps().join(";");
// 				qDebug() <<"B";
				for ( int t = 0; t < tmplines.count(); ++t )
				{

					QString subplace = tmplines[t];
					for ( QMap<QString,QString>::const_iterator repIt = subfamilyReplace.begin(); repIt != subfamilyReplace.end();   ++repIt )
						subplace.replace ( repIt.key(),repIt.value(),Qt::CaseSensitive );
					sublines << subplace;
				}
// 				qDebug() <<"C";
				double available =  paperHeight - thePos.y();
				double needed = ( sublines.count() * conSubfamily[elemIndex].textStyle.lineheight )
				                + conSubfamily[elemIndex].textStyle.margin_top
				                + conSubfamily[elemIndex].textStyle.margin_bottom;
// 				qDebug() <<"D";
				if ( needed > available )
				{
					/// We are in a PAGE context
// 					qDebug() << "NSPAGE";
					// close, clean and create
					theScene.render ( &thePainter );

					thePos.ry() = 0;
					for ( int  d = 0; d <  renderedFont.count() ; ++d )
						renderedFont[d]->deRenderAll();
					for ( int  d = 0; d < renderedGraphic.count(); ++d)
						delete renderedGraphic[d];
					renderedFont.clear();
					renderedGraphic.clear();
					theScene.removeItem ( theScene.createItemGroup ( theScene.items() ) );

					thePrinter.newPage();
					pageNumStr.setNum ( ++pageNumber );

					//
					for ( int pIndex = 0; pIndex < conPage.count(); ++pIndex )
					{
						QStringList pagelines ;
						QStringList tmplines = conPage[pIndex].textElement.e.split ( "\n" );
						pageReplace["##PAGENUMBER##"] = pageNumStr;
						pageReplace["##FAMILY##"] = currentFamily;
						pageReplace["##SUBFAMILY##"] = currentSubfamily;
						for ( int t = 0; t < tmplines.count(); ++t )
						{

							QString pageplace = tmplines[t];
							for ( QMap<QString,QString>::const_iterator repIt = pageReplace.begin(); repIt != pageReplace.end();   ++repIt )
								pageplace.replace ( repIt.key(),repIt.value(),Qt::CaseSensitive );
							pagelines << pageplace;
						}
// 							QFont pFont ( conPage[pIndex].textStyle.font, conPage[pIndex].textStyle.fontsize );
						for ( int pl = 0; pl < pagelines.count(); ++pl )
						{
							QGraphicsTextItem * ti = theScene.addText ( pagelines[pl], qfontCache[conPage[pIndex].textStyle.name] );
							ti->setPos ( conPage[pIndex].textStyle.margin_left, conPage[pIndex].textStyle.margin_top + ( pl * conPage[pIndex].textStyle.lineheight ) );
							ti->setZValue(10);
							ti->setDefaultTextColor(conPage[pIndex].textStyle.color);
						}
						if(conPage[pIndex].graphic.valid)
						{
							QGraphicsSvgItem *svgIt = new QGraphicsSvgItem();
							svgIt->setSharedRenderer(svgRendered[conPage[pIndex].graphic.name]);
							theScene.addItem(svgIt);
							svgIt->setPos(conPage[pIndex].graphic.x, conPage[pIndex].graphic.y);
							renderedGraphic << svgIt;
							svgIt->setZValue(1);
						}
					}
				}
// 				else
// 					qDebug() << "NO_NSPAGE";

				if ( conSubfamily[elemIndex].textStyle.font == "_FONTMATRIX_" ) // We’ll use the current font
				{
					for ( int sl = 0; sl < sublines.count(); ++sl )
					{
						QPointF pen ( conSubfamily[elemIndex].textStyle.margin_left,
						              thePos.y() +
						              ( conSubfamily[elemIndex].textStyle.margin_top + ( ( sl + 1 ) * conSubfamily[elemIndex].textStyle.lineheight ) ) );
						theFont->renderLine ( &theScene,
						                      sublines[sl],
						                      pen ,
						                      conSubfamily[elemIndex].textStyle.fontsize, 10000 );
						//TODO adding color support for text sample
					}
					renderedFont.append ( theFont );
				}
				else
				{
// 						QFont aFont ( conSubfamily[elemIndex].textStyle.font,conSubfamily[elemIndex].textStyle.fontsize );
					for ( int sl = 0; sl < sublines.count(); ++sl )
					{
						QGraphicsTextItem * ti = theScene.addText ( sublines[sl], qfontCache[ conSubfamily[elemIndex].textStyle.name] );
						ti->setPos ( conSubfamily[elemIndex].textStyle.margin_left, thePos.y() + ( conSubfamily[elemIndex].textStyle.margin_top + ( sl * conSubfamily[elemIndex].textStyle.lineheight ) ) );
						ti->setZValue(10000);
						ti->setDefaultTextColor(conSubfamily[elemIndex].textStyle.color);
					}
				}

				if(conSubfamily[elemIndex].graphic.valid)
				{
					QGraphicsSvgItem *svgIt = new QGraphicsSvgItem();
					svgIt->setSharedRenderer(svgRendered[conSubfamily[elemIndex].graphic.name]);
					theScene.addItem(svgIt);
					svgIt->setPos(conSubfamily[elemIndex].graphic.x, conSubfamily[elemIndex].graphic.y + thePos.y());
					renderedGraphic << svgIt;
					svgIt->setZValue(100000);
				}
				theFont->setFTRaster ( oldRast );
				thePos.ry() += needed;
				
			} // end of SUBFAMILY level elements
// 			qDebug() << "ENDOF_SUBFAMILY";
		}

		
	}
	if ( renderedFont.count() )
	{
		theScene.render ( &thePainter );
		for ( int  d = 0; d <  renderedFont.count() ; ++d )
		{
			renderedFont[d]->deRenderAll();

		}
		for ( int  d = 0; d < renderedGraphic.count(); ++d)
			delete renderedGraphic[d];
		renderedFont.clear();
		renderedGraphic.clear();

	}
}

