#########################################################
##	onfire
##
FxInfo  

LifeSpan	300

Condition
	On 	Time
	Time 	0

	
	Event
		Type	Local 
		At	chest
		
		Sound ExploDisplayCase2 90 90 .87

	End

	Event
		EName 	Base
		Type	Local 
		At	Root

		BhvrOverride
			PositionOffset	0 3 0
		End
		
		#Sound	telekinetic3 80 30 .9

	End

	Event
		EName 	case
		Type	Local 
		At	Base

		BhvrOverride
			PositionOffset	0 3 0
		End

	End

End

Condition
	Time 	0

	Event
		Type	start
		At	Root
		Geom	_shpmgc_box_sm04
		
		Bhvr	V_COV\PhysicsEnabled\MetalChunkScatter.bhvr
		BhvrOverride
			PositionOffset		0 3.1 0
			pyrRotateJitter		0 0 0
			InitialVelocity		.0 0.05 .0
			InitialVelocityJitter	0.015 0.015 0.015
			Scale			1 1 1
			physics 		0
			Gravity			0
			Spin			0 .001 0
			SpinJitter		0 .1 0

		End
		
		Part	MISSIONITEMS/missionitem_highlight2.part
		Part	Events\Valentines\StunStream.part
		Part	Events\Valentines\PowerUpMainStreamRays.part
		#Part	Events\Valentines\PowerUpMainStreamChunks01.part

		Lifespan	75
	End

End

Condition
	On 	Time
	Time 	0
	Repeat	1

	Event
		Type	start 
		At	Base
	
		
		Part	Explosions\MetalExplosion\PHS_MetalExplosionBig.part

	
		Part	Explosions\MetalExplosion\PHS_MetalExplosion01.part

		
		Part	Explosions\MetalExplosion\PHS_MetalExplosion02.part
	End

End

Condition
	On 	Time
	Time 	0
	Repeat	1

	Event
		Type	start 
		At	Base
		
		BhvrOverride
			InitialVelocity		0.0 0.28 0.0		
			InitialVelocityJitter	0.1 0.2 0.1
			Drag			0.006
			physics 		1
			physRadius 		0.12
			physGravity 		.9
			physRestitution 	0.5
			physSFriction 	 	0.3
			physDFriction 	 	0.5
			Gravity 		.04
		End

		
		Part	Explosions\MetalExplosion\PHS_MetalBits.part

	End

End

Condition
	On 	Time
	Time 	0
	Repeat	1

	Event
		Type	start 
		At	Base
		
		BhvrOverride
		
			InitialVelocity		0.0 0.28 0.0		
			InitialVelocityJitter	0.1 0.2 0.1		
			Drag			0.006
			physics 		1
			physRadius 		0.12
			physGravity 		.9
			physRestitution 	0.5
			physSFriction 	 	0.3
			physDFriction 	 	0.5
			Gravity 		.04
		End

		Part	Explosions\MetalExplosion\PHS_MetalBits2.part

	End

End

Condition
	Time 	0
	Chance	.9

	Event
		Type	start
		At	Base
		Geom	Metal_Curl
		Geom	Metal_Flat
		
		Bhvr	Behaviors\MetalOffset.bhvr
		BhvrOverride
			PyrRotateJitter		180 180 180
			Drag			0.006
			physics 		1
			physRadius 		0.8
			physGravity 		.9
			physRestitution 	0.5
			physSFriction 	 	0.3
			physDFriction 	 	0.5
			FadeOutLength		1110
		End
		CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001
		#Part1	Powers/FireControl/BurningMeteor.part
		Lifespan	1120
		LifespanJitter	30
	End

End

Condition
	Time 	0
	Chance	.9

	Event
		Type	start
		At	Base
		Geom	Metal_Curl
		Geom	Metal_Flat
		Bhvr	Behaviors\MetalOffset2.bhvr
		BhvrOverride
			PyrRotateJitter		180 180 180
			Drag			0.006
			physics 		1
			physRadius 		0.8
			physGravity 		.9
			physRestitution 	0.5
			physSFriction 	 	0.3
			physDFriction 	 	0.5
			FadeOutLength		1110
		End
		CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001
		#Part1	Powers/FireControl/BurningMeteor.part
		Lifespan	1120
		LifespanJitter	30
	End

End

Condition
	Time 	0
	Chance	.9

	Event
		Type	start
		At	Base
		Geom	Metal_Curl
		Geom	Metal_Flat
		Bhvr	Behaviors\MetalOffset3.bhvr
		BhvrOverride
			PyrRotateJitter		180 180 180
			Drag			0.006
			physics 		1
			physRadius 		0.8
			physGravity 		.9
			physRestitution 	0.5
			physSFriction 	 	0.3
			physDFriction 	 	0.5
			FadeOutLength		1110
		End
		CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001
		#Part1	Powers/FireControl/BurningMeteor.part
		Lifespan	1120
		LifespanJitter	30
	End

End

Condition
	Time 	0
	Chance	.9

	Event
		Type	start
		At	Base
		Geom	Metal_Curl
		Geom	Metal_Flat
		Bhvr	Behaviors\MetalOffset4.bhvr
		BhvrOverride
			PyrRotateJitter		180 180 180
			Drag			0.006
			physics 		1
			physRadius 		0.8
			physGravity 		.9
			physRestitution 	0.5
			physSFriction 	 	0.3
			physDFriction 	 	0.5
			FadeOutLength		1110
		End
		CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001
		#Part1	Powers/FireControl/BurningMeteor.part
		Lifespan	1120
		LifespanJitter	30
	End

End

Condition
	Time 	0
	Chance	.9

	Event
		Type	start
		At	Base
		Geom	Metal_Curl
		Geom	Metal_Flat
		Bhvr	Behaviors\MetalOffset5.bhvr
		BhvrOverride
			PyrRotateJitter		180 180 180
			Drag			0.006
			physics 		1
			physRadius 		0.8
			physGravity 		.9
			physRestitution 	0.5
			physSFriction 	 	0.3
			physDFriction 	 	0.5
			FadeOutLength		1110
		End
		CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001
		#Part1	Powers/FireControl/BurningMeteor.part
		Lifespan	1120
		LifespanJitter	30
	End

End

Condition
	Time 	0
	Repeat	6

	Event
		Type	start
		At	Base
		Geom	Metal_Curl
		Geom	Metal_Flat
		
		Bhvr	V_COV\PhysicsEnabled\MetalChunkScatter.bhvr
		BhvrOverride
			InitialVelocity		.0 0.3 .0
			InitialVelocityJitter	0.5 0.2 0.5
			Drag			0.006
			physics 		1
			physRadius 		0.8
			physGravity 		.9
			physRestitution 	0.5
			physSFriction 	 	0.3
			physDFriction 	 	0.5
			FadeOutLength		1110
		End
		CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001
		#Part1	Powers/FireControl/BurningMeteor.part
		Lifespan	1120
		LifespanJitter	30
	End

End

###################  Glass #####################

Condition
	On 	Time
	Time 	0

	Event
		Ename	Center
		Type	Start 
		At	case
		LifeSpan 	5
		#Sound	Ice7 100 80 .6
		Part1	Explosions\GlassExplosion\GlassDust.part
		Part1	Explosions\GlassExplosion\GlassDustExplosion.part
	End

End

Condition
	On 	Time
	Time 	0
	Repeat 		12
	RepeatJitter	3

	Event
		Type	Start 
		At	Center
		LifeSpan 	150
		ChildFX Explosions\GlassExplosion\GlassCube.fx
	End

End

Condition
	On 	Time
	Time 	0
	Repeat 		8
	RepeatJitter	2

	Event
		Type	Start 
		At	Center
		LifeSpan 	100
		ChildFX	Explosions\GlassExplosion\GlassShardExplode.fx
	End

End

Condition
	On 	Time
	Time 	0
	Repeat 		4
	RepeatJitter	1

	Event
		Type	Start 
		At	Center
		LifeSpan 	100
		ChildFX	Explosions\GlassExplosion\GlassPanelSmall.fx
	End

End


End			