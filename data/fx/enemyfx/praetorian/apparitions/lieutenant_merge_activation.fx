#############################################################
## Turret_MuzzleFlash.fx
#############################################################

FxInfo
LifeSpan 70

#############################################################

Condition
	On Time
	Time 12
	Event
		Type	Local
		At	Root
		Sound 	Apparitions_Merge_01 100 100 .8
		Sound 	Apparitions_Merge_02 100 100 .8
		Sound 	Apparitions_Merge_03 100 100 .8
	End
End

Condition
	On	Cycle
	Time	12

	Event
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset	0.0 6.0 0.0
		End
		ChildFX	:Lieutenant_DispersedImpression_Flash_Merged.fx
		Lifespan 5
	End
End


Condition
	On 	Time
	Time	60

	Event
		Type	Local
		At	Root
		Sound 	Apparitions_MergeLieut_Hit_01 200 100 1
		Sound 	Apparitions_MergeLieut_Hit_02 200 100 1
	End

	Event
		Type	Local
		At	Root
		Bhvr	behaviors\CameraShake01Subtle.bhvr
		BhvrOverride
			PositionOffset	0.0 2.5 0.0
		End
		Part	:Explosion_Shockwave_Lieu.part
		Part	:Explosion_Flash_Large.part
		Part	:Explosion_Smoke_Lieu.part

		Lifespan 6
	End

End

#############################################################
