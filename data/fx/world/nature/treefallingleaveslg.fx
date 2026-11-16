#########################################################
##	TreeFallingLeavesLG
##
FxInfo

Input  
	InpName	Origin
End

#Condition
#	On 	Time
#	Time 	0
#
#	Event
#		EName 	Prime
#		Type	Local 
#		At	Origin
#		Part1	WORLD/Nature/FallingLeavesLG.part
#	End
#End
#

Condition
	On 	Cycle
	Chance	.25
	Time		17
	
	RepeatJitter	1

	Event
		HardwareOnly
		Type	start
		At	Origin

		Geom	Geo_leaf_maple

		
		Bhvr	Explosions\LeafExplosion\GeoLeaf.bhvr
		BhvrOverride
			scale			1 1 1
			SpinJitter		.2 .3 .1
			StartJitter		9 3 9
			InitialVelocity		.0 -0.2 .0
			InitialVelocityJitter	0.6 0.3 0.6
			PyrRotateJitter		180 180 180
			Drag			0.025
			physics 		1
			physRadius 		1.8
			physGravity 		.5
			Gravity			0.02
			physRestitution 	0.1
			physSFriction 	 	0.8
			physDFriction 	 	0.5
			FadeOutLength		150
			
			StartColor		180 160 140

			PhysDebris		1
			PhysDensity		2
		End

		Lifespan	3
	End

End

Condition
	On 	Cycle
	Chance	.2
	Time		11
	
	RepeatJitter	1

	Event
		Type	start
		At	Origin

		Geom	Geo_leaf_maple

		
		Bhvr	Explosions\LeafExplosion\GeoLeaf.bhvr
		BhvrOverride
			scale			1 1 1
			SpinJitter		.2 .3 .1
			StartJitter		9 3 9
			InitialVelocity		.0 -0.2 .0
			InitialVelocityJitter	0.6 0.3 0.6
			PyrRotateJitter		180 180 180
			Drag			0.025
			physics 		1
			physRadius 		1.8
			physGravity 		.5
			Gravity			0.02
			physRestitution 	0.1
			physSFriction 	 	0.8
			physDFriction 	 	0.5
			FadeOutLength		150
			
			StartColor		180 160 140

			PhysDebris		1
			PhysDensity		2
		End

		Lifespan	3
	End

End

Condition
	On 	Cycle
	Chance	.15
	Time		9
	
	RepeatJitter	1

	Event
		SoftwareOnly
		Type	start
		At	Origin

		Geom	Geo_leaf_maple

		
		Bhvr	Explosions\LeafExplosion\GeoLeaf.bhvr
		BhvrOverride
			scale			.6 .6 .6
			SpinJitter		.2 .3 .1
			StartJitter		8 2 8
			InitialVelocity		.0 -0.2 .0
			InitialVelocityJitter	0.4 0.3 0.4
			PyrRotateJitter		180 180 180
			Drag			0.025
			
			StartColor		180 160 140

			Gravity			0.01

			FadeOutLength		150

			Physics			0
		End

		Lifespan	120
	End

	Event
		HardwareOnly
		Type	start
		At	Origin

		Geom	Geo_leaf_maple

		
		Bhvr	Explosions\LeafExplosion\GeoLeaf.bhvr
		BhvrOverride
			scale			1 1 1
			SpinJitter		.2 .3 .1
			StartJitter		9 3 9
			InitialVelocity		.0 -0.2 .0
			InitialVelocityJitter	0.6 0.3 0.6
			PyrRotateJitter		180 180 180
			Drag			0.025
			physics 		1
			physRadius 		1.8
			physGravity 		.5
			Gravity			0.02
			physRestitution 	0.1
			physSFriction 	 	0.8
			physDFriction 	 	0.5
			FadeOutLength		150
			
			StartColor		180 160 140

			PhysDebris		1
			PhysDensity		2
		End

		Lifespan	3
	End
End


End			