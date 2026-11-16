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
		Bhvr	behaviors\CameraShake01Subtle.bhvr
		BhvrOverride
			PositionOffset	0.0 2.5 0.0
		End
		Part	:Explosion_Flash_Minion.part
		Part	:Explosion_Smoke_Minion.part
		Part	:Explosion_Smoke_Lingering_Minion.part

		Lifespan 6
	End

End

#############################################################
