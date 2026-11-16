#############################################################
## FX System Name
#############################################################

FxInfo

Flags		InheritAlpha

LifeSpan	220

#############################################################

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

	Event
		Ename	TargetNode_Starboard
		Type	local
		At	Hips
		BhvrOverride
			PositionOffset	-64 -32 12
		End
		#Geom	GEO_FX_TestSphere
	End
End

Condition

	Event
		Ename	TargetNode_Port
		Type	local
		At	Hips
		BhvrOverride
			PositionOffset	64 -32 12
		End
		#Geom	GEO_FX_TestSphere
	End
End

#############################################################

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
		At	ULegR
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
		At	LLegR
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
		At	FootR
		ChildFX	:EnergyBurst\EnergyBurst_SmallBeam_Starboard.fx
		LifeSpan	20
	End
End

##############################################################

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

Condition
	On	Time
	Time	15

	Event
		Ename	Prime2
		Type	Local
		At	ULegL
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
		At	LLegL
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
		At	FootL
		ChildFX	:EnergyBurst\EnergyBurst_SmallBeam_Port.fx
		LifeSpan	25
	End
End

#############################################################

Condition
	On	Time
	Time	15

	Event
		Ename	Prime2
		Type	Local
		At	ULegR
		ChildFX	:EnergyBurst\EnergyBurst_SmallBeam_Starboard.fx
		LifeSpan	35
	End
End

Condition
	On	Time
	Time	20

	Event
		Ename	Prime2
		Type	Local
		At	LLegR
		ChildFX	:EnergyBurst\EnergyBurst_SmallBeam_Starboard.fx
		LifeSpan	30
	End
End

Condition
	On	Time
	Time	25

	Event
		Ename	Prime2
		Type	Local
		At	FootR
		ChildFX	:EnergyBurst\EnergyBurst_SmallBeam_Starboard.fx
		LifeSpan	25
	End
End

#############################################################

End		