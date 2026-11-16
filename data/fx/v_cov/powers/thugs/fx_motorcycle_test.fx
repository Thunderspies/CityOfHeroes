#########################################################
##	pump_smoke
##
FxInfo

Flags InheritAnimScale

LifeSpan 150

###########  crate  ############################

Condition
	On	Time
	Time	0

	Event
		EName	BikeParent
		Type	Local
		At	Mystic

		#Geom	Testing_Target
		Lifespan 98
	End
End


Condition
	On	Time
	Time	0


	Event
		EName	Bike
		Type	Local
		At	BikeParent
		BhvrOverride
			PositionOffset		0 -1.656 -.98
		End

		bhvr	V_COV\Powers\Thugs\Thugs_B_FadeinMoto.bhvr
		Anim	FX_Motorcycle
		#Geom	Testing_Target
	End
End

#Condition
#	On	Time
#	Time	99
#
#
#	Event
#		EName	Bike01
#		Type	Start
#		At	Bike
##		BhvrOverride
##			PositionOffset		0 -1.656 -.98
##		End
#
#		bhvr	V_COV\Powers\Thugs\Thugs_B_FlashIn.bhvr
#		Anim	FX_Motorcycle
##		Geom	Testing_Target
#		Lifespan 100
#	End
#End
#

End