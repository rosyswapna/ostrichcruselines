<?php
/**********************************************************************
    Copyright (C) FrontAccounting, LLC.
	Released under the terms of the GNU General Public License, GPL, 
	as published by the Free Software Foundation, either version 3 
	of the License, or (at your option) any later version.
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  
    See the License here <http://www.gnu.org/licenses/gpl-3.0.html>.
***********************************************************************/

/**
	Document blueprint use following parameters set in doctext.inc:

	$Addr1,	$Addr2 - address info
	$Payment_Terms - payment terms line
	$Footer - footer texts
	$this->company - company info
	$this->title - report title
	$this->formData - some other info
***/

		$this->row = $this->pageHeight - $this->topMargin;

		$upper = $this->row - 2 * $this->lineHeight;
		$lower = $this->bottomMargin + 8 * $this->lineHeight;
		$iline1 = $upper - 7.5 * $this->lineHeight;
		$iline2 = $iline1 - 1.5 * $this->lineHeight;
		$iline3 = $iline2 - 8 * $this->lineHeight;
		$iline4 = $iline3 - 1.5 * $this->lineHeight;

		$right = $this->pageWidth - $this->rightMargin;
		$width = ($right - $this->leftMargin) / 5;
		$icol = $this->pageWidth / 2;
		$ccol = $this->cols[0] + 4;
		$c2col = $ccol + 60;
		$ccol2 = $icol / 2;
		$mcol = $icol + 8;
		$mcol2 = $this->pageWidth - $ccol2;
		$cols = count($this->cols);
		   
		$this->Line($iline1);
		$this->Line($iline2);
		$this->Line($iline3);
		$this->Line($iline4);
		
		$col = $this->leftMargin;
		for ($i = 0; $i < $cols; $i++)
		{
			//$this->LineTo($this->cols[$i] , $iline1 ,$this->cols[$i], $iline4);
		}
		
		$this->LineTo($this->leftMargin, $iline1 ,$this->leftMargin, $iline4);

		$this->LineTo($this->cols[1] , $iline1 ,$this->cols[1], $iline3);
		$this->LineTo($this->cols[2] , $iline1 ,$this->cols[2], $iline3);
		$this->LineTo($this->cols[4] , $iline1 ,$this->cols[4], $iline3);
		$this->LineTo($this->cols[5] , $iline1 ,$this->cols[5], $iline3);
		$this->LineTo($this->cols[6] , $iline1 ,$this->cols[6], $iline4);


		$this->LineTo($right, $iline1 ,$right, $iline4);

		// Company Logo
		$this->NewLine();
		$logo = company_path() . "/images/" . $this->company['coy_logo'];
		if ($this->company['coy_logo'] != '' && file_exists($logo))
		{
			$this->AddImage($logo, $ccol, $this->row, 0, 40);
		}
		else
		{
			$this->fontSize += 4;
			$this->Font('bold');
			$this->Text($ccol, $this->company['coy_name'], $icol);
			$this->Font();
			$this->fontSize -= 4;
		}
		// Document title
		$this->fontSize += 10;
		$this->Font('bold');
		
		$this->TextWrap($ccol, $this->row, $this->pageWidth - $this->rightMargin , $this->title, 'C');
		$this->Font();
		$this->fontSize -= 10;
		$this->NewLine();
		$adrline = $this->row;

		// Company data
		$this->TextWrapLines($ccol, $icol, $this->company['postal_address']);
		$this->Font('italic');
		if (@$this->company['phone'])
		{
			$this->Text($ccol, _("Phone"), $c2col);
			$this->Text($c2col, $this->company['phone'], $mcol);
			$this->NewLine();
		}
		if (@$this->company['fax'])
		{
			$this->Text($ccol, _("Fax"), $c2col);
			$this->Text($c2col, $this->company['fax'], $mcol);
			$this->NewLine();
		}
		if (@$this->company['email'])
		{
			$this->Text($ccol, _("Email"), $c2col);

			$url = "mailto:" . $this->company['email'];
			$this->SetTextColor(0, 0, 255);
			$this->Text($c2col, $this->company['email'], $mcol);
			$this->SetTextColor(0, 0, 0);
			$this->addLink($url, $c2col, $this->row, $mcol, $this->row + $this->lineHeight);

			$this->NewLine();
		}
		if (@$this->company['gst_no'])
		{
			$this->Text($ccol, _("Our VAT No."), $c2col);
			$this->Text($c2col, $this->company['gst_no'], $mcol);
			$this->NewLine();
		}
		if (@$this->formData['domicile'])
		{
			$this->Text($ccol, _("Domicile"), $c2col);
			$this->Text($c2col, $this->company['domicile'], $mcol);
			$this->NewLine();
		}
		$this->Font();
		$this->row = $adrline;
		//$this->NewLine(3);
		$this->Text($mcol + 100, _("Date"));
		$this->Text($mcol + 180, sql2date($this->formData['document_date']));

		$this->NewLine();
		$this->Text($mcol + 100, $this->formData['document_name']);
		$this->Text($mcol + 180, $this->formData['document_number']);

		$this->NewLine();
		$this->Text($mcol + 100, $Addr2['name']);
		$this->NewLine();
		$this->TextWrapLines($mcol + 100, $this->rightMargin - $mcol, $Addr1['address']);


		// Line headers
		$this->row = $iline1 - $this->lineHeight - 1;
		$this->Font('bold');
		$count = count($this->headers);
		$this->cols[$count] = $right - 3;
		for ($i = 0; $i < $count; $i++)
			$this->TextCol($i, $i + 1, $this->headers[$i], -2);
		$this->Font();

		// Footer
		$this->Font('italic');
		$this->row = $iline4 - $this->lineHeight - 6;
		$sign_row = $this->row;
		foreach ($Footer as $line => $txt)
		{
			if (!is_numeric($line))	// title => link
			{
				$this->fontSize -= 2;
				$this->TextWrap($ccol, $this->row, $right - $ccol, $line, 'left');
				$this->row -= $this->lineHeight;
				$this->SetTextColor(0, 0, 255);
				$this->TextWrap($ccol, $this->row, $right - $ccol, $txt, 'left');
				$this->SetTextColor(0, 0, 0);
				$this->addLink($txt, $ccol, $this->row, $this->pageWidth - $this->rightMargin, $this->row + $this->lineHeight);
				$this->fontSize += 2;
			}
			else
				$this->TextWrap($ccol, $this->row, $this->cols[4], $txt, 'left');
			$this->row -= $this->lineHeight;

		}
		$this->Font();
		$this->fontSize -= 1;
		$this->TextWrap($this->cols[5], $sign_row, $this->cols[8], "For ".$this->company['coy_name'], 'left');
		$this->fontSize += 1;
		$this->TextWrap($this->cols[5], $sign_row-50, $this->cols[5], "Authorized Signatory", 'left');
		
		
		$temp = $iline2 - $this->lineHeight - 2;
		$temp1 = $iline3 - $this->lineHeight - 2;
		
?>
