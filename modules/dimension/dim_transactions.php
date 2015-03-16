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

$page_security = 'SA_DIMENSIONTX';
$path_to_root = "../..";

include($path_to_root . "/includes/db_pager.inc");
include_once($path_to_root . "/includes/session.inc");

include_once($path_to_root . "/includes/date_functions.inc");
include_once($path_to_root . "/includes/ui.inc");
$js = "";
if ($use_popup_windows)
	$js .= get_js_open_window(800, 500);
if ($use_date_picker)
	$js .= get_js_date_picker();

add_access_extensions();
set_ext_domain('modules/dimension');
$_SESSION['page_title'] = _($help_context = "Dimension Transaction Inquiry");
page($_SESSION['page_title'], false, false, "", $js);

if (get_post('SearchTx'))
{
	$Ajax->activate('dimx_table');
}

function view_item($row){
	$item = '';
	if($row['type'] == ST_SALESINVOICE){
		$result = get_customer_trans_details($row['type'],$row['type_no']);
		while($row = db_fetch($result)){
			$item[] = $row['description'];
		}
	}elseif($row['type'] == ST_SUPPINVOICE){
		$result = get_supp_invoice_items($row['type'],$row['type_no']);
		while($row = db_fetch($result)){
			$item[] = $row['description'];
		}
	}

	return implode(',',$item);
}

function view_narration($row)
{
	return 'narration';
}

function fmt_debit($row)
{
	$value = -$row["amount"];
	return $value>=0 ? price_format($value) : '';

	

}

function fmt_credit($row)
{
	$value = $row["amount"];
	return $value>=0 ? price_format($value) : '';
}

function view_balance($pager)
{
	$rows = $pager->data;
	$colspan = 5;
	$balance = 0;
	foreach($rows as $row){
		$balance += $row['amount'];
	}
	$balance_row[] = array('Balance','colspan='.$colspan." align=right");
	$balance_row[] = array(price_format($balance),"align=right");
	return $balance_row;
	
}



start_form(false, false, $_SERVER['PHP_SELF']);
	start_table(TABLESTYLE_NOBORDER);
	start_row();
		dimensions_list_cells(_("Dimension")." 1:", 'dimension_id', $_POST['dimension_id'], true, "All Dimensions ", false, 1);
		submit_cells('SearchTx', _("Search"), '', '', 'default');
	end_row();

	end_table();
end_form();

$dim = get_company_pref('use_dimension');

$sql = get_sql_for_dimension_tx($dim);

$cols = array(
	_("Date")=>array('type'=>'date'),
	_("Item") => array('fun'=>'view_item'), 
	_("Narration") , 
	_("Account"), 
	_("Debit") => array('align'=>'right', 'fun'=>'fmt_debit'), 
	_("Credit") => array('align'=>'right', 'fun'=>'fmt_credit'),
	
);

$table =& new_db_pager('dimx_tbl', $sql, $cols);

$table->width = "80%";
$table->set_footer('view_balance','tableheader');

display_db_pager($table);


end_page();


