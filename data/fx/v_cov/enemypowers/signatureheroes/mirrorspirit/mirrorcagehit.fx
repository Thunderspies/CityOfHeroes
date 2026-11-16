#########################################################
##	Mirror Hit
########################################################
FxInfo

LifeSpan  90
############################################


Condition
	On 	Time
	Time 	0

	
	Event
		Ename	Flash
		Type	Start
		At	Chest
		
		bhvr	behaviors/GenericParticleFade.bhvr

		Part	:MirrorSparkGlow.part
		Part	:MirrorFlash.part
		Part	:MirrorFlash.part
		Part	:MirrorSmokeTrail_BIG.part
		Part	:MirrorSparkTrail_BIG.part
		sound	mirrorhit2 77 66 .97
		Lifespan	8
	End

End

Condition
	On 	Time
	Time 	0

	Repeat	8
	RepeatJitter	2

	Event
		Ename	Chunks
		Type	Start
		At	Chest

		
		Part	:MirrorSparkTrail.part

		Geom	FX_BronzeShard_01
		Geom	FX_BronzeMirrorShard_01
		Geom	FX_BronzeShard_02
		Geom	FX_BronzeMirrorShard_02
		Geom	FX_BronzeShard_03
		Geom	FX_BronzeMirrorShard_03
		Geom	FX_BronzeShard_04
		Geom	FX_BronzeMirrorShard_04

		BhvrOverride
			Scale			.4 .4 .4
			pyrRotateJitter		180 180 180
			SpinJitter		.5 .5 .5
			InitialVelocity		.0 0.3 .0
			InitialVelocityJitter	0.5 0.2 0.5
			Drag			0.006
			Gravity			0.03
			
		End

		Lifespan	90
	End

End


Condition
	On 	Time
	Time 	0
	
	Repeat	5
	RepeatJitter	2
	
	Event
		EName	Sparkup
		Type 	Start
		At	Chest
		ChildFX	V_COV\PhysicsEnabled\MetalSparkUP.fx
		
	End

End

Condition
	On 	Time
	Time 	0

	Repeat	6
	RepeatJitter	3
	
	Event
		EName	Sparkfar
		Type 	Start
		At	Chest
		ChildFX	V_COV\PhysicsEnabled\MetalSparkOUT.fx
		
	End

End


End
