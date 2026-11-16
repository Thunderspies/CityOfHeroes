#############################################################
## Omega Device - Spawn - Teleport In
#############################################################

FxInfo

LifeSpan 90

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	Start
		At	Root
		Bhvr	behaviors\fadein.bhvr
		BhvrOverride
			FadeInLength		60
			positionoffset		0 10 0
			spin 			0 .15 0
		End
		part	V_COV\EnemyPowers\Arachnos\Flyer_Critter\Flyer_deploySparkTube.part
		Flags AdoptParentEntity
	End

	Event
		Type	Start
		At	Root
		BhvrOverride
			PyrRotate	180 0 0
			Spin		0 0.15 0
		End
		part	V_COV\EnemyPowers\Arachnos\Flyer_Critter\Flyer_deploySparkTube.part
		Sound shieldspawner 80 80 .9
	End
End

#############################################################

End