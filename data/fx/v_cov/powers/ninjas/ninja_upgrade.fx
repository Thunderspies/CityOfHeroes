
FxInfo

Flags	InheritAnimScale

Lifespan 250








####### draw wep  #################################


Condition
	On	Time
	Time	86

	Event
		EName	prop
		Type	Local
		At	WepL
		Geom	GEO_WepL_Katana_3
		Lifespan	62
	End
End


#######  target  Node  ################################


Condition
	On	Time
	Time	70

	Event
		EName	CatchTarget
		Type	Local
		At	T_Root

		bhvr	V_COV\Powers\Ninjas\Ninja_Upgrade_offset.bhvr
		#Geom	Testing_Target
	End
End


#######  toss katana  #################################

Condition
	On	Time
	Time	148

	Event
		EName	Prime
		Type	Start
		At	WepL

		bhvr	V_COV\Powers\Paramilitary\Paramilitary_B_BoostTravel.bhvr
		Geom	GEO_WepL_Katana_3
		Magnet	CatchTarget
		LookAt	CatchTarget
	End
End


############################################################
Condition
	On	PrimeHit

	Event
		EName	Prime
		Type	Destroy
	End


End



End