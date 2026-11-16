#############################################################
## FX_Fish_01_leaping.fx
#############################################################

FxInfo
Lifespan 20

#############################################################

Condition
	On	Time
	Time	0

	Event
		EName	OffsetGeo
		Type	Start
		At	Root
		Bhvr	Behaviors/GenericParticleFade.bhvr
		BhvrOVerride
			Alpha 1
		End
		Geom	FX_FishLeapOffset
	End
End


Condition
	On 	Time
	Time 	11

	Event
		Type	Local
		At	OffsetGeo
		AltPiv	1
		Part	WORLD\tunnels\Bottom_Splash_Small.part
		Lifespan 2
	End
End

Condition
	On 	Time
	Time 	20

	Event
		Type	Local
		At	OffsetGeo
		AltPiv	2
		BhvrOVerride
			PositionOffset	0 2 0
		End
		Part	WORLD\Nature\Wildlife\Bottom_Splash_Secondary_Sm.part
		Part	WORLD\tunnels\Splash_Small_Thick01.part
		Lifespan 1
	End
End
###########################################################

End
