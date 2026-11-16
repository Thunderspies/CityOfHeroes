#########################################################
##	iceweapon
########################################################
FxInfo

Input
	Inpname	Origin
End

Lifespan 90

########################################################

#Condition
#	On 	Time
#	Time 	0
#
#	Event
#		EName	Main
#		Type	local
#		At	Root
#		Anim	V_Coral_Guardian_Boss_Tintable
#		BhvrOverride
#			Alpha 1
#		End
#		Lifespan	85
#	End
#End

Condition
	On	Time
	Time	10

	Event
		EName 	Anchor_LLegR
		Type	PositOnly
		At	UlegR
		#AnimPiv	UlegR
		Lifespan	100
	End

	Event
		EName 	Anchor_LLegL
		Type	PositOnly
		At	LLegL
		#AnimPiv	LLegL
		Lifespan	100
	End

	Event
		EName 	Anchor_HandR
		Type	PositOnly
		At	HandR
		#AnimPiv	HandR
		Lifespan	100
	End

	Event
		EName 	Anchor_HandL
		Type	PositOnly
		At	HandL
		#AnimPiv	HandL
		Lifespan	100
	End

	Event
		EName	Anchor_LArmR
		Type	PositOnly
		At	LArmR
		#AnimPiv	LArmR
		Lifespan	100
	End

	Event
		Type	PositOnly
		At	LarmR
		#AnimPiv	LarmR
		Part    :V_Mako_WaterDrips.part
		pother	Anchor_HandR
		Lifespan	40
	End

	Event

		Type	PositOnly
		At	UArmR
		#AnimPiv UArmR
		Part    :V_Mako_WaterDrips.part
		pother	Anchor_LArmR
		Lifespan	40
	End

	Event
		Type	PositOnly
		At	UlegR
		#AnimPiv	UlegR
		Part    :V_Mako_WaterDrips.part
		pother	Anchor_LLegR
		Lifespan	40
	End

	Event
		EName	Anchor_LArmL
		Type	PositOnly
		At	LarmL
		#AnimPiv	LarmL

		Part    :V_Mako_WaterDrips.part
		pother	Anchor_HandL
		Lifespan	40
	End

	Event
		Type	PositOnly
		At	UarmL
		#AnimPiv	UarmL

		Part    :V_Mako_WaterDrips.part
		pother	Anchor_LArmL
		Lifespan	40
	End


	Event
		Type	PositOnly
		At	ULegL
		#AnimPiv	ULegL
		Part    :V_Mako_WaterDrips.part
		POther	Anchor_LLegL
		Lifespan	40
	End




End

Condition
	On 	Time
	Time 	15

	Event
		EName	Target_UArmR
		Type	PositOnly
		At	UArmR
		#AnimPiv	UArmR
		Part    :V_Mako_WaterDrips.part
		Lifespan	60
	End

	Event
		EName	Anchor_LArmR
		Type	PositOnly
		At	LArmR
		#AnimPiv	LArmR
		Part    :V_Mako_WaterDrips.part
		Lifespan	60
	End

	Event
		Type	PositOnly
		At	UlegR
		#AnimPiv	UlegR
		Part    :V_Mako_WaterDrips.part
		pother	Anchor_LLegR
		Lifespan	60
	End


	Event
		Type	PositOnly
		At	UArmL
		#AnimPiv	UArmL
		Part    :V_Mako_WaterDrips.part
		pother	Anchor_LArmL
		Lifespan	60
	End

	Event
		Type	PositOnly
		At	LarmL
		#AnimPiv	LarmL
		Part    :V_Mako_WaterDrips.part
		pother	Anchor_HandL
		Lifespan	60
	End

	Event
		Type	PositOnly
		At	UlegL
		#AnimPiv	UlegL
		Part    :V_Mako_WaterDrips.part
		pother	Anchor_LLegL
		Lifespan	60
	End
End

########################################################

End
