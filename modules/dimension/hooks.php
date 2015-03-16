<?php
// ----------------------------------------------------------------
// Module name: DIMENSIONS TRANSACTIONS LIST
// Author: Swapna
// ----------------------------------------------------------------

class hooks_dimension extends hooks {
	var $module_name = 'dimension';

    function install_options($app) {
        global $path_to_root;

        switch($app->id) {
           
            case 'proj':
                $app->add_lapp_function(1, _('&Dimension Transactions Inquiry'), $path_to_root.'/modules/dimension/dim_transactions.php', 'SA_DIMENSIONTX',
                    MENU_INQUIRY);                
                break;
        }
    }

    function install_access()
    {

        $security_sections[SS_DIM] = _("Dimension");

        $security_areas['SA_DIMENSIONTX'] = array(SS_DIM|1, _("Dimension Transactions Inquiry"));
        
        return array($security_areas, $security_sections);
    }

/* This method is called on extension activation for company.   */
    function activate_extension($company, $check_only=true)
    {
        global $db_connections;

        $updates = array(
            'update_dimension_db.sql' => array('dimension_widgets')
        );

        return $this->update_databases($company, $updates, $check_only);
    }

    function deactivate_extension($company, $check_only=true)
    {
        global $db_connections;

        $updates = array(
            'drop_dimension_db.sql' => array('ugly_hack') // FIXME: just an ugly hack to clean database on deactivation
        );

        return $this->update_databases($company, $updates, $check_only);
    }
}

?>
