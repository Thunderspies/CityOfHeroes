#########################################################
##	onfire
##
FxInfo  


#Condition
#	On 	Time
#	Time 	0
#	DoMany	1
#
#	Event
#		Ename	Origin
#		Type	Local 
#		At	Origin
#		BhvrOverride
#			PositionOffset		0 5 0
#		End
#	
#	End
#End

###################   Barrels   #################################

Condition
	On 	Time
	Time 	0
	DoMany	1
	
	Repeat	5
	RepeatJitter	2

	Event
		EName 	Bounce
		Type	Start
		At	Origin

		Geom	OilDrum
		Geom	JunkBarrel_55gal_01
		Geom	JunkBarrel_55gal_02
		Geom	OilDrum
		Geom	JunkBarrel_55gal_02

		Bhvr	V_COV\PhysicsEnabled\JunkScatter.bhvr
		BhvrOverride
			PositionOffset		0 -4.5 0
			InitialVelocity		0 0.25 0
			InitialVelocityJitter	.5 0.5 .5

			physSFriction 	 	0.3
			physDFriction 	 	0.5
			physRestitution 	0.3
		End
	End
	
	Event
		HardwareOnly
		EName 	Bounce
		Type	Start
		At	Origin

		Geom	OilDrum
		Geom	JunkBarrel_55gal_01
		Geom	JunkBarrel_55gal_02
		Geom	OilDrum
		Geom	JunkBarrel_55gal_02

		Bhvr	V_COV\PhysicsEnabled\JunkScatter.bhvr
		BhvrOverride
			PositionOffset		0 -4.5 0
			InitialVelocity		0 0.25 0
			InitialVelocityJitter	.5 0.5 .5

			physSFriction 	 	0.3
			physDFriction 	 	0.5
			physRestitution 	0.3
		End
	End

End

###################   Big Stuff   #################################

Condition
	On 	Time
	Time 	0
	DoMany	1
	
	Repeat	4
	RepeatJitter	2

	Event
		EName 	Bounce
		Type	Start
		At	Origin

		Geom	barstool
		Geom	Med_Air_Tank_03
		Geom	bartable
		Geom	FileCabinet_Sm_01
		Geom	Chair_Generic_Wait_01
		Geom	TrashCan_Sm

		Bhvr	V_COV\PhysicsEnabled\JunkScatter.bhvr
		BhvrOverride
			PositionOffset		0 -4.5 0
			InitialVelocity		0 0.4 0
			InitialVelocityJitter	.5 0.5 .5

			physRestitution 	0.3
		End
	End
	
	Event
		HardwareOnly
		EName 	Bounce
		Type	Start
		At	Origin

		Geom	barstool
		Geom	Med_Air_Tank_03
		Geom	bartable
		Geom	FileCabinet_Sm_01
		Geom	Chair_Generic_Wait_01
		Geom	TrashCan_Sm

		Bhvr	V_COV\PhysicsEnabled\JunkScatter.bhvr
		BhvrOverride
			PositionOffset		0 -4.5 0
			InitialVelocity		0 0.4 0
			InitialVelocityJitter	.5 0.5 .5

			physRestitution 	0.3
		End
	End

End

###################   Too Big Stuff   #################################

Condition
	On 	Time
	Time 	0
	DoMany	1
	
	Repeat	3
	RepeatJitter	2

	Event
		EName 	Bounce
		Type	Start
		At	Origin

		Geom	FileCabinet_Lg_01
		Geom	CardBrdBox_genrc_01
		Geom	CardBrdBox_genrc_05
		Geom	CardBrdBox_genrc_08
		Geom	CardBrdBox_genrc_09

		Bhvr	V_COV\PhysicsEnabled\JunkScatter.bhvr
		BhvrOverride
			PositionOffset		0 -4.5 0
			InitialVelocity		0 0.4 0
			InitialVelocityJitter	.5 0.5 .5

			Scale			.75 .75 .75
			StartColor		150 150 150

			physRestitution 	0.3
		End
	End
	
	Event
		HardwareOnly
		EName 	Bounce
		Type	Start
		At	Origin

		Geom	FileCabinet_Lg_01
		Geom	CardBrdBox_genrc_01
		Geom	CardBrdBox_genrc_05
		Geom	CardBrdBox_genrc_08
		Geom	CardBrdBox_genrc_09

		Bhvr	V_COV\PhysicsEnabled\JunkScatter.bhvr
		BhvrOverride
			PositionOffset		0 -4.5 0
			InitialVelocity		0 0.4 0
			InitialVelocityJitter	.5 0.5 .5

			Scale			.75 .75 .75
			StartColor		150 150 150

			physRestitution 	0.3
		End
	End

End
###################   TRASH   #################################

Condition
	On 	Time
	Time 	0
	DoMany	1
	
	Repeat	12
	RepeatJitter	5

	Event
		Type	start
		At	Origin
		
		geom	GEO_Slag_Golem_Junk_19
		geom	GEO_Slag_Golem_Junk_34
		geom	GEO_Slag_Golem_Junk_35
		geom	GEO_Slag_Golem_Junk_39
		geom	GEO_Slag_Golem_Junk_40
		geom	GEO_Slag_Golem_Junk_44
		
		Bhvr	V_COV\PhysicsEnabled\JunkScatter.bhvr #Explosions\TrashExplosion\TrashDebrisFade.bhvr
		BhvrOverride
			PositionOffset		0 -4.5 0
			StartJitter		2 1 2		
			scale			1 1 1

			InitialVelocity		0 0.6 0
			InitialVelocityJitter	.5 0.5 .5
			StartColor		200 190 180

			PyrRotateJitter		180 180 180
			spinJitter		.2 .3 .2

			Drag			0.025

			physics 		1
			physRadius 		0.8
			physGravity 		.75
			physRestitution 	0.5
			physSFriction 	 	0.7
			physDFriction 	 	0.6
			PhysDensity		3
		End
	End

	Event
		HardwareOnly
		Type	start
		At	Origin

		geom	GEO_Slag_Golem_Junk_01
		geom	GEO_Slag_Golem_Junk_02
		geom	GEO_Slag_Golem_Junk_14
		geom	GEO_Slag_Golem_Junk_26
		geom	GEO_Slag_Golem_Junk_45
		
		Geom	GEO_Slag_Golem_Junk_13

		Bhvr	V_COV\PhysicsEnabled\JunkScatter.bhvr #Explosions\TrashExplosion\TrashDebrisFade.bhvr
		BhvrOverride
			PositionOffset		0 -4.5 0
			StartJitter		2 1 2		
			scale			1 1 1

			InitialVelocity		0 0.6 0
			InitialVelocityJitter	.74 0.74 .74
			StartColor		200 190 180

			PyrRotateJitter		180 180 180
			spinJitter		.2 .3 .2

			Drag			0.025

			physics 		1
			physRadius 		0.8
			physGravity 		1
			physRestitution 	0.3
			physSFriction 	 	0.9
			physDFriction 	 	0.8
			PhysDensity		2
		End
	End
End

Condition
	On 	Time
	Time 	0
	DoMany	1

	Repeat	16
	RepeatJitter	8

	Event
		Type	start 
		At	Origin
		
		Geom	newpaper01
		Geom	newpaper02
		Geom	newpaper03
		
		Bhvr	V_COV\PhysicsEnabled\JunkScatter.bhvr
		
		BhvrOverride
			PositionOffset		0 -4.5 0
			StartJitter		2 1 2

			scale			.5 .5 .5
			
			InitialVelocity		.0 0.75 .0
			InitialVelocityJitter	.5 1 .5

			PyrRotateJitter		180 180 180
			SpinJitter		0.2 0.2 0.2

			Drag			0.05

			physics 		1
			physRadius 		3
			physGravity 		.75
			physRestitution 	0.15
			physSFriction 	 	0.7
			physDFriction 	 	0.7
			Gravity 		.02

			PhysDensity		2
			PhysScale		1.2 2 1.2

		End


	End
End

Condition
	On 	Time
	Time 	0
	DoMany	1
	
	Repeat	6
	RepeatJitter	3

	Event
		Type	start
		At	Origin

		geom	GEO_Slag_Golem_Junk_01
		geom	GEO_Slag_Golem_Junk_02
		geom	GEO_Slag_Golem_Junk_14
		geom	GEO_Slag_Golem_Junk_26
		geom	GEO_Slag_Golem_Junk_45
		
		Geom	GEO_Slag_Golem_Junk_13

		Bhvr	V_COV\PhysicsEnabled\JunkScatter.bhvr #Explosions\TrashExplosion\TrashDebrisFade.bhvr
		BhvrOverride
			PositionOffset		0 -4.5 0
			StartJitter		2 1 2		
			scale			1 1 1

			InitialVelocity		0 0.5 0
			InitialVelocityJitter	.4 0.4 .4
			StartColor		200 190 180

			PyrRotateJitter		180 180 180
			spinJitter		.2 .3 .2

			Drag			0.025

			physics 		1
			physRadius 		0.8
			physGravity 		1
			physRestitution 	0.3
			physSFriction 	 	0.9
			physDFriction 	 	0.8
			PhysDensity		2
		End
	End

End

###################   TRASH   #################################

End			