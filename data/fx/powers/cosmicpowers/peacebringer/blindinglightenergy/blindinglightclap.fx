#############################################################
## FX System Name
#############################################################

FxInfo

Lifespan 200

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local 
		At	Origin
		BHVR	Behaviors/OffsetEyes.bhvr
		ChildFX	POWERS\CosmicPowers\Peacebringer\ActivationEyes\ActivationEyes_Child.fx
		LifeSpan	75
	End
End

#############################################################

Condition
	On 	Time
	Time	0
	
	Event	
		Type	Local
		At 	Origin
		Sound PeaceBlindingLight 100 100 1.0
	End
End

Condition

	On 	Time	
	Time	56

	Event	
		ENAME	Overhead
		Type	Local
		At	Root
		Geom	OverHeadDummy
		LifeSpan	15	
	End

	Event	
		ENAME	Flash
		Type	Local
		At	Overhead
		AltPiv	1
		part	:SSTClapFlash.part
		part	:BrightLightTendrils.part
		LifeSpan	12	
	End
End

Condition

	On 	Time	
	Time	56

	Event	
		ENAME	ThunderShake1
		Type	Local
		At	Chest
		Bhvr	Behaviors/CameraShake02.bhvr
		#LifeSpan	70
	End
End

#############################################################

End