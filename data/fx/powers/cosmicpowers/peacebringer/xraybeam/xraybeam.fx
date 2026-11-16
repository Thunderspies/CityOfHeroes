#############################################################
## FX System Name
#############################################################

FxInfo
	
LifeSpan	200

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local 
		At	Origin
		BHVR	Behaviors/OffsetEyes.bhvr
		ChildFX	POWERS\CosmicPowers\Peacebringer\ActivationEyes\ActivationEyes_Child.fx
		LifeSpan	45
	End
End

######################################################################

Condition
	On	Time 
	Time	0

	Event
		EName	core
		Type	Local
		At	head
		Geom	LocalPivot01	
		bhvr	behaviors/PsionicsScale3.bhvr
	End

	Event
		Type	Local
		At	core
		altPiv	1
		Bhvr	behaviors/XRayBeam.bhvr
		Part	:XRayEyeGlow.Part
		Part	:XRayEyeRings.Part
	End

	Event
		Type	Local
		At	core
		altPiv	3
		Bhvr	behaviors/XRayBeam.bhvr
		Part	:XRayEyeGlow.Part
		Part	:XRayEyeRings.Part
	End
End

Condition

	On	Time 
	Time	25

	Event
		Type	Local
		At	core
		altPiv	1
		Bhvr	behaviors/XRayBeamEyes.bhvr
		Part	:NictusBlueElectricity.part
		Part	:NictusBlueElectricity.part
		Lifespan	15
	End

	Event
		Type	Local
		At	core
		altPiv	3
		Bhvr	behaviors/XRayBeamEyes.bhvr
		Part	:NictusBlueElectricity.part
		Part	:NictusBlueElectricity.part
		Lifespan	15
	End
End

Condition
	On	Time
	Time	12

	Event
		Type	Local
		At	Core
		Sound xray6 80 80 .88
	End
End

Condition
	On	Time
	Time	29

	Event
		Type	Local
		At	Core
		Altpiv	1
		Part	:XRayEyeFlash.Part
		Part	:XRayEyeFlash2.Part
	End

	Event
		Type	Local
		At	Core
		Altpiv	3
		Part	:XRayEyeFlash.Part
		Part	:XRayEyeFlash2.Part
	End
End

Condition
	On	Time
	Time	30

	Event
		EName 	Prime4
		Type	Start 
		At	core
		altpiv	1
		Bhvr	:Fastprojectile4.bhvr
		Part	:XRayBeamGlow.Part
		Part	:XRayBeamGlow.Part
		Part	:XRayBeamGlow.Part
		Magnet	T_Chest
		LookAt	T_Chest
	End

	Event
		EName 	Prime
		Type	Start 
		At	core
		altpiv	3
		Bhvr	:Fastprojectile4.bhvr
		Part	:XRayBeamGlow.Part
		Part	:XRayBeamGlow.Part
		Part	:XRayBeamGlow.Part
		Magnet	T_Chest
		LookAt	T_Chest
	End

	Event
		EName	Prime1
		Type	start
		At	core
		Altpiv	1
		Bhvr	behaviors/XRayBeam.bhvr
		geom	XrayBeamPeace
		Magnet	T_Chest
		LookAt	T_Chest
		LifeSpan	12
	End

	Event
		EName	Prime2
		Type	start
		At	core
		Altpiv	3
		Bhvr	behaviors/XRayBeam.bhvr
		geom	XrayBeamPeace
		Magnet	T_Chest
		LookAt	T_Chest
		LifeSpan	12
	End
End

Condition
	On	PrimeHit
		
	Event
		Ename	Prime
		Type	Destroy
	End
End

Condition
	On	Prime1Hit
		
	Event
		Ename	Prime1
		Type	Destroy
	End
End

Condition
	On	Prime2Hit
		
	Event
		Ename	Prime2
		Type	Destroy
	End
End

Condition
	On	Prime4Hit
		
	Event
		Ename	Prime4
		Type	Destroy
	End
End

######################################################################

End			