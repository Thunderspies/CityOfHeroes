#########################################################
##	template

FxInfo

Flags InheritAnimScale InheritAlpha
LifeSpan 50


################# toung   out  #######################

Condition
	On 	Time
	Time 	18

	Event
		EName	Prime     ###Toung
		Type	Posit
		At	BendMystic  ##Chest
		Bhvr	behaviors/Toung_Strike.bhvr
		geom	Toung_twist_3
##		anim	FX_Toung
		Magnet	T_Chest
		LookAt	T_Chest
		Lifespan	5

	End

End

########  reverse toung  ################################

Condition
	On 	Time
	Time 	25

	Event
		EName	Prime2     ###Toung
		Type	Posit
		At	BendMystic  ##Chest
		Bhvr	behaviors/Toung_Strike_reverse.bhvr
		geom	Toung_twist_3
		Magnet	T_Chest
		LookAt	T_Chest
		Sound HordelingTongue2 80 80 .8
		Lifespan	5
		
	End

End

########  test marker  ################################

#Condition
#	On 	Time
#	Time 	0
#
#	Event
#		EName	test
#		Type	Posit
#		At	BendMystic  ##Chest
#		geom	Toung_twist_3
#		Magnet	T_Chest
#		LookAt	T_Chest
#
#	End
#
#End






End



		