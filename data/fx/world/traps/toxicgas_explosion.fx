#############################################################
## ToxicGas_Explosion.fx
#############################################################

FxInfo
Lifespan 150

#############################################################

Condition
	On 	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound 	Sewer_GasExplosion_01 100 100 .8
	End
End

Condition
	On	Time
	Time	0

	Event
		Ename	Prime
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset	0 0 0 		#0 0 10
		End
	End
End

Condition
	On	Time
	Time	0	#30

	Event
		Type	Local
		At	Prime
		Bhvr	behaviors/CameraShake02.bhvr
		BhvrOverride
			PositionOffset	0 0 1
		End
		part	ENEMYFX\V_Signature\Maelstrom\CoreExplosion_Flash_Initial.part
		part	ENEMYFX\V_Signature\Maelstrom\CoreExplosion_Flash_Big.part
		Part	ENEMYFX\Signature\Duray\Dirt_Splatter.part
		Part	ENEMYFX\Signature\Duray\Flash_Glow_Large.part
		Part	ENEMYFX\Signature\Duray\Flash_Sparks.part
		Lifespan	5
	End
End

Condition
	On	Time
	Time	0	#30
	Repeat	8

	Event
		Type	start
		At	Prime
		Bhvr	Behaviors\FadeOut.bhvr
		BhvrOverride
			StartColor		100 100 100
			InitialVelocity		0.0 0.9 0.0
			InitialVelocityJitter	0.75 0.5 0.75
			Scale			0.5 0.5 0.5
			SpinJitter		0.5 0.5 0.5
			physics 		1
			physRadius 		0.8
			physGravity 		1.2
			physRestitution 	0.5
			physSFriction 	 	0.3
			physDFriction 	 	0.5
			FadeOutLength		200
			PhysDensity		2
		End
		Geom	ShrapnelA
		Geom	ShrapnelC
		Geom	ShrapnelD
		Geom	ShrapnelF
		CEvent	ENEMYFX\V_Signature\Maelstrom\CHILD_Sparks_Shrapnel.fx
		CThresh 0.1
		Lifespan	250
	End

	Event
		Type	Local
		At	Prime
		BhvrOverride
			PositionOffset	0 0.1 1
		End
		Part	:ExplosionScorchMark.part
	End
End

Condition
	On	Time
	Time	2	#30
	Event
		Ename	ToxicGas
		Type	Local
		At	Prime
		BhvrOverride
			PositionOffset	0 2 1
		End
		Part	:ToxicGas_Out.part
		Part	:ToxicGas_OutHilight.part
		Lifespan 10
	End
End
#############################################################
End