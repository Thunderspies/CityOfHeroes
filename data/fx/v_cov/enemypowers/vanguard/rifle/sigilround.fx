#########################################################
## Vanguard "Redding" Rifle - Sigil Round
#########################################################

FxInfo
LifeSpan  300

## MUZZLEFLASH #######################################################

Condition
	On 	Time
	Time 	45

	Event
		ENAME   AltPivots
		Type	Local 
		At	WepR
		Geom	Vanguard_Rifle
	End
	Event
		Type	start
		At	AltPivots
		Altpiv	1
		Sound advancedassault 100.0 100.0 1.0
	End

	Event
		Type	Local 
		At	WepR
		ChildFX	V_COV\PhysicsEnabled\ARGunShellEject.fx
	End


	Event
		Type	Local
		At	AltPivots
		Altpiv	1
		Part2	WEAPONFX/Gunfire_Shotgunsmoke.part
		Part3	WEAPONFX/Gunfire_ShotgunMuzzflashSmall.part
		lifespan	3
	End

	Event
		Type	Local
		At	AltPivots
		Altpiv	1
		Part1	WEAPONFX/TorchflameAb.part
		Part2	WEAPONFX/TorchflameBa.part
		Geom	FX_MachineGunBurst
		lifespan	1
	End
End

## TRACER #######################################################

Condition
	On 	Time
	Time 	47

	Event
		EName 	Prime
		Type	Start
		At	AltPivots
		Altpiv	1
		Bhvr	:SigilroundTracer\Tracer.bhvr
		Part	:SigilroundTracer\TracerTrail01.part
		Magnet	Target
		Lookat	Target
		Lifespan	30
	End

	Event
		EName 	TracerHelix
		Type	Start
		At	AltPivots
		Altpiv	1
		Bhvr	:SigilroundTracer\Helix.bhvr
		Geom	FX_Vanguard_SigilRoundTracer__SigilRoundTracer
		Magnet	Target
		Lookat	Target
	End

	Event
		EName 	Prime2
		Type	Start
		At	AltPivots
		Altpiv	1
		Bhvr	:SigilroundTracer\Tracer.bhvr
		BhvrOverride
			TrackRate		4
		End
		Magnet	Target
		Lookat	Target
		Lifespan	30
	End

	Event
		EName 	Tracer_Rings
		Type	Local 
		At	Prime2
		part1	:SigilroundTracer\Rings01.part
		part1	:SigilroundTracer\Rings02.part
		Lifespan	20
	End
End

## CLEAN UP #######################################################

#Condition
#	On	PrimeHit
#			
#	Event
#		Ename	Prime
#		Type	Destroy
#	End
#End

#########################################################

End