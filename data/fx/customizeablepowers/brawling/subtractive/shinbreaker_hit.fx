#############################################################
## HEADER
#############################################################

FxInfo

Lifespan 100

#############################################################

Condition
	On 	Time
	Time	2
	Event
		Type	Local
		At	Root
		Sound 	Brawling_ShinBreaker_Hit_01 100 100 .8
	End
End

Condition
	On	Time
	Time	2

	Event
		Type	local
		At	Root
		BhvrOverride
			PositionOffset	0 1.0 0.0
		End
		Part	:Head_Shockwave_Small.part
		Part	:Head_Spikes.part
		Part	:Hit_Sparks_Small.part
		LifeSpan 2
	End

	Event
		Type	local
		At	Root
		Bhvr	behaviors/Dust_Expanding.bhvr
		Part	:Hit_Smoke_Radial.part
		Part	:Hit_Smoke_Radial_Lingering.part

		LifeSpan 20
	End
End


#############################################################

End