#############################################################
## HEADER
#############################################################

FxInfo

Lifespan 100

#############################################################

Condition
	On 	Time
	Time	5
	Event
		Type	Local
		At	Root
		Sound 	Brawling_RibCracker_Attack_01 200 100 .8
	End
End

Condition
	On	Time
	Time	13

	Event
		Type	local
		At	Root
		BhvrOverride
			PYRRotate	135 0 90
		End
		Part	:Knee_Trail.part
		Part	:Knee_Trail_Additive.part

		LifeSpan 1
	End
	Event
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset	0 1.0 0.0
			PYRRotate	0 90 -135

		End
		Part	:Knee_Trail.part
		Part	:Knee_Trail_Additive.part
		Lifespan 1
	End

	Event
		Type	local
		At	Root
		BhvrOverride
			PositionOffset	0.0 0.0 1.0
			PYRRotate	135 0 90
		End
		Part	:Knee_Dust.part
		LifeSpan 8
	End
End

#############################################################

End