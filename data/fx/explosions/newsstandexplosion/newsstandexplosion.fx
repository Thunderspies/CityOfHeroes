#########################################################
##	onfire
##
FxInfo  

LifeSpan	1100

Condition
	On 	Time
	Time 	0

	Event
		EName 	Prime1
		Type	Local 
		At	chest
		
		Sound newspaperexplo1 80 80 .7
		
	End

End

#Condition
#	On 	Time
#	Time 	0
#	Repeat	10
#
#	Event
#		Type	local 
#		At	chest
#		Bhvr	Behaviors/Fade.bhvr
#		Part	world/City/Newspaper1.part
#		Part	world/City/Newspaper2.part
#
#		LifeSpan	10
#	End
#
#End

Condition
	On 	Time
	Time 	0
	Repeat	1

	Event
		Type	start 
		At	chest
		
		Part	Explosions\MetalExplosion\PHS_MetalExplosionBig.part

		Part	Explosions\MetalExplosion\PHS_MetalExplosion01.part

	End

End

Condition
	On 	Time
	Time 	0

	Event
		Type	start
		At	chest
		Geom	Broken_News_Stand
		
		Bhvr	V_COV\PhysicsEnabled\MetalChunkScatter.bhvr
		BhvrOverride
			StartJitter		0 0 0		
			scale			1.2 1.2 1.2

			InitialVelocity		0 0.5 0
			InitialVelocityJitter	0.25 0.15 0.25

			StartColor		140 130 120
			PyrRotateJitter		0 0 0

			Drag			0.006
			physics 		1
			physRadius 		0.8
			physGravity 		1
			physRestitution 	0.1
			physSFriction 	 	0.3
			physDFriction 	 	0.5
			FadeOutLength		510

			PhysDebris		3
		End

		#CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001

		Lifespan	11
	End

	Event	
		HardwareOnly
		Type	start
		At	Chest

		ChildFx		Explosions\ChangeExplosion\ChangeExplosion.Fx

	End
End

Condition
	Time 	0
	Repeat	8
	RepeatJitter	4

	Event
		Type	start
		At	Chest
		Geom	FX_newspaper
		
		Bhvr	V_COV\PhysicsEnabled\MetalChunkScatter.bhvr
		BhvrOverride
			StartJitter		0 0 0		
			scale			1.2 1.2 1.2
			InitialVelocity		.0 0.4 .0
			InitialVelocityJitter	0.15 0.2 0.15
			PyrRotateJitter		180 180 180

			StartColor		180 200 220

			Drag			0.006
			physics 		1
			physRadius 		0.8
			physGravity 		.8
			physRestitution 	0.1
			physSFriction 	 	0.5
			physDFriction 	 	0.5
			FadeOutLength		510
			
			PhysDebris		1
			PhysDensity		2.5
		End

		Lifespan	11
		LifespanJitter	3
	End

	
	Event
		HardwareOnly
		Type	start
		At	Chest
		Geom	FX_newspaper
		
		Bhvr	V_COV\PhysicsEnabled\MetalChunkScatter.bhvr
		BhvrOverride
			StartJitter		0 0 0		
			scale			1.2 1.2 1.2
			InitialVelocity		.0 0.4 .0
			InitialVelocityJitter	0.15 0.2 0.15
			PyrRotateJitter		180 180 180

			StartColor		180 200 220

			Drag			0.006
			physics 		1
			physRadius 		0.8
			physGravity 		.8
			physRestitution 	0.1
			physSFriction 	 	0.5
			physDFriction 	 	0.5
			FadeOutLength		510
			PhysDebris		1
			PhysDensity		2.5
		End

		Lifespan	11
		LifespanJitter	3
	End

End


Condition
	Time 	0
	Repeat	8
	RepeatJitter	4

	Event	
		Type	start
		At	Chest
		Geom	newpaper01
		Geom	newpaper02
		Geom	newpaper03
		
		Bhvr	V_COV\PhysicsEnabled\MetalChunkScatter.bhvr
		BhvrOverride
			StartJitter		0 0 0		
			scale			.8 .8 .8
			InitialVelocity		.0 0.4 .0
			InitialVelocityJitter	0.25 0.35 0.25
			PyrRotateJitter		180 180 180
			Drag			0.026
			physics 		1
			physRadius 		0.8
			physGravity 		.3
			physRestitution 	0.1
			physSFriction 	 	0.5
			physDFriction 	 	0.5
			FadeOutLength		510
			
			PhysDebris		1
			PhysDensity		3
			PhysScale		1.2 2 1.2
		End
		
			Lifespan	10
	
	End

	
	Event	
		HardwareOnly
		Type	start
		At	Chest
		Geom	newpaper01
		Geom	newpaper02
		Geom	newpaper03
		
		Bhvr	V_COV\PhysicsEnabled\MetalChunkScatter.bhvr
		BhvrOverride
			StartJitter		0 0 0		
			scale			.8 .8 .8
			InitialVelocity		.0 0.4 .0
			InitialVelocityJitter	0.25 0.35 0.25
			PyrRotateJitter		180 180 180
			Drag			0.026
			physics 		1
			physRadius 		0.8
			physGravity 		.3
			physRestitution 	0.1
			physSFriction 	 	0.5
			physDFriction 	 	0.5
			FadeOutLength		510
			
			PhysDebris		1
			PhysDensity		3
			PhysScale		1.2 2 1.2
		End
		
			Lifespan	10
	
	End


End

End			