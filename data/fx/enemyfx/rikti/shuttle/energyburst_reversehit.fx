#############################################################
## FX System Name
#############################################################

FxInfo

LifeSpan	220

## OFFSET TARGET NODES ###########################################################

Condition

	Event
		Ename	TargetNode_Starboard
		Type	local
		At	T_Hips
		BhvrOverride
			PositionOffset	-64 -32 12
		End
	End
End

Condition

	Event
		Ename	TargetNode_Port
		Type	local
		At	T_Hips
		BhvrOverride
			PositionOffset	64 -32 12
		End
	End
End

Condition

	On 	Time	
	Time	0

	Event
		Type Local
		At origin
		Sound EBursts3 500 500 .5
	End

End

Condition
	On	Time
	Time	15

	Event
		Ename	Prime2
		Type	Local
		At	TargetNode_Starboard
		Bhvr	:EnergyBurst\BeamSphere.bhvr
		Geom	GEO_WhiteSphere
		part	:EnergyBurst\LightningArcs1.part
		part	:EnergyBurst\LightningGlow1.part
		part	:EnergyBurst\LightningArcs2.part
		part	:EnergyBurst\LightningGlow2.part
		part	:EnergyBurst\LightningArcs3.part
		Lifespan	40
	End
End

Condition
	On	Time
	Time	15

	Event
		Ename	Prime2
		Type	Local
		At	TargetNode_Port
		Bhvr	:EnergyBurst\BeamSphere.bhvr
		Geom	GEO_WhiteSphere
		part	:EnergyBurst\LightningArcs1.part
		part	:EnergyBurst\LightningGlow1.part
		part	:EnergyBurst\LightningArcs2.part
		part	:EnergyBurst\LightningGlow2.part
		part	:EnergyBurst\LightningArcs3.part
		Lifespan	40
	End
End
## SMALL BEAMS - STARBOARD ###########################################################

Condition
	On	Time
	Time	15

	Event
		Ename	Prime2
		Type	Local
		At	T_ULegR
		ChildFX	:EnergyBurst\EnergyBurst_SmallBeam_Starboard.fx
		LifeSpan	30
	End
End

Condition
	On	Time
	Time	20

	Event
		Ename	Prime2
		Type	Local
		At	T_LLegR
		ChildFX	:EnergyBurst\EnergyBurst_SmallBeam_Starboard.fx
		LifeSpan	25
	End
End

Condition
	On	Time
	Time	25

	Event
		Ename	Prime2
		Type	Local
		At	T_FootR
		ChildFX	:EnergyBurst\EnergyBurst_SmallBeam_Starboard.fx
		LifeSpan	20
	End
End

## SMALL BEAMS - PORT ############################################################

Condition
	On	Time
	Time	15

	Event
		Ename	Prime2
		Type	Local
		At	T_ULegL
		ChildFX	:EnergyBurst\EnergyBurst_SmallBeam_Port.fx
		LifeSpan	35
	End
End

Condition
	On	Time
	Time	20

	Event
		Ename	Prime2
		Type	Local
		At	T_LLegL
		ChildFX	:EnergyBurst\EnergyBurst_SmallBeam_Port.fx
		LifeSpan	30
	End
End

Condition
	On	Time
	Time	25

	Event
		Ename	Prime2
		Type	Local
		At	T_FootL
		ChildFX	:EnergyBurst\EnergyBurst_SmallBeam_Port.fx
		LifeSpan	25
	End
End

## LARGE BEAM ###########################################################

Condition
	On	Time
	Time	35

	Event
		Ename	Prime2
		Type	Local
		At	TargetNode_Starboard
		ChildFX	:EnergyBurst\EnergyBurst_LargeBeam.fx
		LifeSpan	20
	End
End

Condition
	On	Time
	Time	35

	Event
		Ename	Prime2
		Type	Local
		At	TargetNode_Port
		ChildFX	:EnergyBurst\EnergyBurst_LargeBeam.fx
		LifeSpan	20
	End
End

## HIT FX ###########################################################

Condition

	On 	Time	
	Time	35

	Event
		Type Local
		At origin
		Sound EBursts_ReverseHit 500 500 .7
	End

End

Condition
	On	Time
	Time	35

	Event
		Ename	Prime2
		Type	Local
		At	Origin
		part	:EnergyBurst\LightningArcs1.part
		part	:EnergyBurst\LightningGlow1.part
		part	:EnergyBurst\LightningArcs2.part
		part	:EnergyBurst\LightningGlow2.part
		part	:EnergyBurst\LightningArcs3.part
		Lifespan	25
	End
End

#############################################################

End		