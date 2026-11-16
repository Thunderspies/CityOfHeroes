#########################################################
## Umbral Blast - Ebon Eye
#########################################################

FxInfo

LifeSpan	200

## SOUND FX ######################################################

Condition
	On	Time
	Time	6

	Event
		Type	Local
		At	Core
		Sound xray5 80 80 .9
	End
End

## EYE FX ######################################################

Condition
	On	Time
	Time	0

	Event
		EName 	Eyes
		Type	Local 
		At	Root
		ChildFX POWERS\CosmicPowers\Nictus\ActivationEyes\ActivationEyes.fx
	End
End

## BEAM FX ######################################################

Condition
	On Time 
	Time 0

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
	On Time 
	Time 20

	Event
		Type	Local
		At	core
		altPiv	1
		Bhvr	behaviors/XRayBeamEyes.bhvr
		Part	:NictusPurpleElectricity.part
		Part	:NictusBlueElectricity.part
		Lifespan	15
	End

	Event
		Type	Local
		At	core
		altPiv	3
		Bhvr	behaviors/XRayBeamEyes.bhvr
		Part	:NictusPurpleElectricity.part
		Part	:NictusBlueElectricity.part
		Lifespan	15
	End
End

Condition
	On	Time
	Time	24

	Event
		Type	Local
		At	Core
		  Altpiv   1
		Part	:XRayEyeFlash.Part
		Part	:XRayEyeFlash1.Part
		Part	:XRayEyeFlash2.Part
	End

	Event
		Type	Local
		At	Core
		  Altpiv   3
		Part	:XRayEyeFlash.Part
		Part	:XRayEyeFlash1.Part
		Part	:XRayEyeFlash2.Part
	End
End

Condition
	On	Time
	Time	25

	Event
		EName 	Prime4
		Type	Start 
		At	core
		altpiv	1
		Bhvr	:Fastprojectile4.bhvr
		Part	:XRayBeamGlowBlue2.Part
		Part	:XRayBeamGlowPurple2.Part
		Part	:XRayBeamGlow.Part
		Part	:XRayBeamGlow.Part
		Part	:XRayBeamGlow.Part

		Magnet	T_Chest
		LookAt	T_Chest
	End

	Event
		EName 	Prime3
		Type	Start 
		At	core
		altpiv	3
		Bhvr	:Fastprojectile4.bhvr
		Part	:XRayBeamGlowBlue2.Part
		Part	:XRayBeamGlowPurple2.Part
		Part	:XRayBeamGlow.Part
		Part	:XRayBeamGlow.Part
		Part	:XRayBeamGlow.Part

		Magnet	T_Chest
		LookAt	T_Chest
	End

	Event
		EName 	Prime
		Type	Start 
		At	Mystic
		Bhvr	:Fastprojectile4.bhvr
			
		Magnet	T_Chest
		LookAt	T_Chest
	End

	Event
		EName	core2
		Type	start
		At	core
		  Altpiv   1
		Bhvr	behaviors/XRayBeam.bhvr
		#Part	:XRayBeamGlow.Part
		
		geom	XrayBeam

		LifeSpan		12

		Magnet	T_Chest
		LookAt	T_Chest
	End

	Event
		EName	core3
		Type	start
		At	core
		   Altpiv   3
		Bhvr	behaviors/XRayBeam.bhvr
		#Part	:XRayBeamGlow.Part
		
		geom	XrayBeam

		LifeSpan		12
	
		Magnet	T_Chest
		LookAt	T_Chest
	End
End

## CLEAN UP ######################################################

Condition
	On	PrimeHit
		
	Event
		Ename	Prime
		Type	Destroy
	End
End

Condition
	On	Prime3Hit
		
	Event
		Ename	Prime3
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

########################################################

End			