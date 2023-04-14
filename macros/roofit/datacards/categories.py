def get_categories(ma_mass, b_only125):
    categories = ["catma90","catma175", "catma300","chi2h_2"] 
    if ma_mass == "75":
        categories = ["catma60","catma90", "catma300"] 
        if b_only125:
            categories = ["catma60","catma90"] 
    if ma_mass == "125":
        categories = ["catma90","chi2h_2","catma300"] 
        if b_only125:
            categories = ["catma90","chi2h_2"] 
    if ma_mass == "99999":
        categories = ["catma90","catma175", "catma300", "chi2h_2"] 
    if ma_mass == "450":
        categories = ["catma175", "catma300"] 
    if ma_mass == "100":
        categories = ["catma90","chi2h_2","catma300"]
        if b_only125:
            categories = ["catma90","chi2h_2"]
    if ma_mass == "175":
        categories = ["chi2h_2","catma175","catma300"]
        if b_only125:
            categories = ["chi2h_2"]
    if ma_mass == "200":
        categories = ["catma175","catma300","chi2h_2"]
        if b_only125:
            categories = ["chi2h_2"]
    if ma_mass ==  "250":
        categories = ["catma175","catma300"]
    if ma_mass ==  "350":
        categories = ["catma175","catma300"]
    if ma_mass ==  "500":
        categories = ["catma175","catma300"]
    return categories
