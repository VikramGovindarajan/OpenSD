node_items = [["tpres_gues"],
            ["spres_gues"],
            ["ttemp_gues"],
            ["tenth_gues"],
            ["ther_gues","rhomass"],
            ["ther_gues","Q"],
            ["msource"],
            ["esource"],
            ["level"],
            ["watermass"],
            ["volume"]]
face_items = [["vflow_gues"],
              ["velocity"],
              ["ther_gues","rhomass"],
              ["ther_gues","Qth"]]
pump_items = [["Nop"],
                ["vflow_gues"],
                ["Head"]]
vof_node_items = [["volfrac1"],
                  ["stemp_gues"],
                  ["senth_gues"],
                  ["ther_gues","rhomass"],
                  ["hsource"]]
vof_face_items = [["volfrac1"],
                  ["vflow_gues"],
                  ["stemp_gues"],
                  ["ther_gues","rhomass"]]
HT_node_items = ["heat_transfer",
                  "temp_gues",
                  "htc"]
HT_iface_items = []
HS_items = ["dind_spl_nb",
            "dind_nb_pd",
            "dind_pd_spv",
            "uheat_transfer",
            "dheat_transfer"]
orifice_items = [["vflow_gues"],
                ["spres_gues"],
                ["opening"],
                ["ther_gues","rhomass"],
                ["ther_gues","Qth"]]
slug_items = ["ILIQ",
              "w2",
              "JST",
              "JEND",
              "zib2",
              "zit2",
              "TiLb2",
              "TiLt2"]
bubble_items = ["IBUB",
                "Tvap2",
                "V"]
pipe_items = ["mflow"]

class Plot:
    
    def __init__(self):
        pass
    
    def export_to_xml(self):
        pass
    