
FxInfo

Flags    InheritAlpha


####  Goblin Dual reverse daggers  ##############################

Condition
	On 	Time
	Time 	15

	Event
		EName	Prop_2
		Type	Local
		At	WepL
		Geom    GEO_WepR_Knife_1
		Bhvr	Behaviors/Reverse_Wep.bhvr
		Bhvr	Behaviors/Reverse_Grip.bhvr
	End

	Event
		EName	Prop_4
		Type	Local
		At	WepR
		Geom    GEO_WepR_Knife_1
		Bhvr	Behaviors/Reverse_Grip.bhvr

	End


End



####  Goblin Dual swords  ##############################

#Condition
#	On 	Time
#	Time 	0
#
#	Event
#		EName	Prop_2
#		Type	Local
#		At	WepL
#		Geom	GEO_WepR_Sword_3
#		Bhvr	Behaviors/Reverse_Wep.bhvr
#	End
#
#	Event
#		EName	Prop_4
#		Type	Local
#		At	WepR
#		Geom	GEO_WepR_Sword_3
#
#	End
#
#
#End

#####  Shield & Sword  ############################

#Condition
#	On 	Time
#	Time 	0
#
#	Event
#		EName	Prop_3
#		Type	Local
#		At	LarmL
#		Geom	shield_elbow
#	End
#
#	Event
#		EName	Prop_4
#		Type	Local
#		At	WepR
#		Geom	GEO_WepR_Sword_3
#
#	End
#
#
#End

####  Dual Axe  ###################################
#
#Condition
#	On 	Time
#	Time 	0
#
#
#	Event
#		EName	Prop_2
#		Type	Local
#		At	WepL
#		Geom	GEO_WepR_Infernal__Fullbright
#	End
#
#	Event
#		EName	Prop_4
#		Type	Local
#		At	WepR
#		Geom	GEO_WepR_Infernal__Fullbright
#
#	End
#
#
#End

###  Dual Katana  #################################

#Condition
#	On 	Time
#	Time 	0
#
#	Event
#		EName	Prop_2
#		Type	Local
#		At	WepL
#		Geom	GEO_WepL_Katana_2
#	End
#
#
#	Event
#		EName	Prop_4
#		Type	Local
#		At	WepR
#		Geom	GEO_WepR_Katana_2
#
#	End
#
#End
#

###################


End