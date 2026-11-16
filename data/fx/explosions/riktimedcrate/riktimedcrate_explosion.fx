#############################################################
## Rikti Medical Crate Explosion
#############################################################

FxInfo  

LifeSpan	1200

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local 
		At	chest
		Sound metal_explo2 110 110 .9
		Lifespan	100
	End

	Event
		Ename	Base
		Type	Local 
		At	Root
		Geom	RiktiMedCrateAltPivots
	End
End

#############################################################

Condition
	On 	Time
	Time 	1

	Event
		Type	start 
		At	root
		BhvrOverride
			StartJitter		1 1 1
			PositionOffset		0 2 0
		End
		childFx	Explosions\MetalExplosion\MetalExplosionChild.fx
	End
End

Condition
	On 	Time
	Time 	0
	Repeat	4

	Event
		Type	start 
		At	Base
		BhvrOverride
			PositionOffset		0 2 0
			StartJitter		2 2 2
		End
		Part	Explosions\MetalExplosion\PHS_MetalExplosionBig.part
		Part	Explosions\MetalExplosion\PHS_MetalExplosion01.part
	End
End

## CRATE PIECES ###########################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	start
		At	Base
		AltPiv	1
		Geom	Rikti_Med_Crate_Destroyed_01
		Bhvr	Explosions\MetalExplosion\MetalPanelLightFade.bhvr
		BhvrOverride
			InitialVelocity		0 0.75 0
			InitialVelocityJitter	0.25 0.25 0.25
			SpinJitter		0.1 0.2 0.1
		End
		CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001
		Lifespan	1120
		LifespanJitter	30
	End

	Event
		Type	start
		At	Base	
		AltPiv	2
		Geom	Rikti_Med_Crate_Destroyed_02
		Bhvr	Explosions\MetalExplosion\MetalPanelLightFade.bhvr
		BhvrOverride
			InitialVelocity		.4 0.25 0
			InitialVelocityJitter	0.25 0.15 0.25
			Spin			0 0 -.15
			SpinJitter		.2 .1 .1
		End
		CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001
		Lifespan	1120
		LifespanJitter	30
	End

	Event
		Type	start
		At	Base
		AltPiv	3
		Geom	Rikti_Med_Crate_Destroyed_03
		Bhvr	Explosions\MetalExplosion\MetalPanelLightFade.bhvr
		BhvrOverride
			InitialVelocity		-.4 0.25 0
			InitialVelocityJitter	0.25 0.15 0.25
			Spin			0 0 .15
			SpinJitter		.2 .1 .1
		End
		CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001
		Lifespan	1120
		LifespanJitter	30
	End

	Event
		Type	start
		At	Base
		AltPiv	4
		Geom	Rikti_Med_Crate_Destroyed_04
		Bhvr	Explosions\MetalExplosion\MetalPanelLightFade.bhvr
		BhvrOverride
			InitialVelocity		0 0.25 .4
			InitialVelocityJitter	0.25 0.15 0.25
			Spin			.15 0 0
			SpinJitter		.1 .1 .2
		End
		CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001
		Lifespan	1120
		LifespanJitter	30
	End

	Event
		Type	start
		At	Base
		AltPiv	5
		Geom	Rikti_Med_Crate_Destroyed_05
		Bhvr	Explosions\MetalExplosion\MetalPanelLightFade.bhvr
		BhvrOverride
			InitialVelocity		0 0.25 -.4
			InitialVelocityJitter	0.25 0.15 0.25
			Spin			-.15 0 0
			SpinJitter		.1 .1 .2
		End
		CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001
		Lifespan	1120
		LifespanJitter	30
	End
End

## CRATE PIECES ###########################################################

Condition
	On 	Time
	Time 	0

	Event
		HardwareOnly
		Type	start
		At	Base
		AltPiv	6
		Geom	Rikti_Med_Crate_Destroyed_06
		Bhvr	Explosions\MetalExplosion\MetalPanelLightFade.bhvr
		BhvrOverride
			InitialVelocity		0 0.75 0
			InitialVelocityJitter	0.25 0.25 0.25
			SpinJitter		.1 .2 .1
			PhysDebris		3
			PhysDensity		3
		End
		CThresh 0.0001
		Lifespan	11
		LifespanJitter	3
	End

	Event
		HardwareOnly
		Type	start
		At	Base
		AltPiv	7
		Geom	Rikti_Med_Crate_Destroyed_07
		Bhvr	Explosions\MetalExplosion\MetalPanelLightFade.bhvr
		BhvrOverride
			InitialVelocity		.4 0.25 0
			InitialVelocityJitter	0.25 0.15 0.25
			Spin			0 0 -.15
			SpinJitter		.2 .1 .1
			PhysDebris		3
			PhysDensity		3
		End
		#CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001
		Lifespan	11
		LifespanJitter	3
	End

	Event
		HardwareOnly
		Type	start
		At	Base
		AltPiv	8
		Geom	Rikti_Med_Crate_Destroyed_08
		Bhvr	Explosions\MetalExplosion\MetalPanelLightFade.bhvr
		BhvrOverride
			InitialVelocity		-.4 0.25 0
			InitialVelocityJitter	0.25 0.15 0.25
			Spin			0 0 .15
			SpinJitter		.2 .1 .1
			PhysDebris		3
			PhysDensity		3
		End
		CThresh 0.0001
		Lifespan	11
		LifespanJitter	3
	End

	Event
		HardwareOnly
		Type	start
		At	Base
		AltPiv	9
		Geom	Rikti_Med_Crate_Destroyed_09
		Bhvr	Explosions\MetalExplosion\MetalPanelLightFade.bhvr
		BhvrOverride
			InitialVelocity		0 0.25 .4
			InitialVelocityJitter	0.25 0.15 0.25
			Spin			.15 0 0
			SpinJitter		.1 .1 .2
			PhysDebris		3
			PhysDensity		3
		End
		#CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001
		Lifespan	11
		LifespanJitter	3
	End

	Event
		HardwareOnly
		Type	start
		At	Base
		AltPiv	10
		Geom	Rikti_Med_Crate_Destroyed_10
		Bhvr	Explosions\MetalExplosion\MetalPanelLightFade.bhvr
		BhvrOverride
			InitialVelocity		0 0.25 -.4
			InitialVelocityJitter	0.25 0.15 0.25
			Spin			-.15 0 0
			SpinJitter		.1 .1 .2
			PhysDebris		3
			PhysDensity		3
		End
		#CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001
		Lifespan	11
		LifespanJitter	3
	End
End

#############################################################

End			