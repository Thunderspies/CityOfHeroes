#############################################################
## HEADER
#############################################################

FxInfo

Lifespan 100

#############################################################

Condition
	On 	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound 	Brawling_CombatReadiness_01 200 100 .8
	End
End

Condition
	On	Time
	Time	5

	Event
		Type	local
		At	Root
		Bhvr	behaviors/Dust_Expanding.bhvr
		Part	:Hit_Smoke_Radial.part
		Part	:Hit_Smoke_Radial_Lingering.part
		LifeSpan 50
	End
End

#############################################################

End