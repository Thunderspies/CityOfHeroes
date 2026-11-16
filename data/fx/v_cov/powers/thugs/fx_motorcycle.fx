#########################################################
##	pump_smoke
##
FxInfo

Flags InheritAnimScale

LifeSpan 100

###########  crate  ############################

Condition
	On	Time
	Time	0

	Event
		EName	BikeParent
		Type	Start
		At	Root

		bhvr	V_COV\Powers\Thugs\Thugs_B_FadeOUTMoto.bhvr
#		Geom	Testing_Target
		Lifespan 97
	End



	Event
		EName	Bike
		Type	Local  
		At	BikeParent
#		BhvrOverride
#			PositionOffset		0 -1.656 -.98
#		End

		bhvr	V_COV\Powers\Thugs\Thugs_B_FlashOutMoto.bhvr
		Anim	FX_Motorcycle
#		Geom	Testing_Target
		Lifespan 80
	End
End


End