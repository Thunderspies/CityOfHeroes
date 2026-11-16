#########################################################
##	onfire
##
FxInfo  

LifeSpan	1200

Condition
	On 	Time
	Time 	0

	Event
		Type	Local 
		At	chest
		
		Sound mailboxexplo2 80 80 .88

	End

End

Condition
	On 	Time
	Time 	37
	Event
		Type	Local 
		At	chest
		
		Sound mailboxhit1 80 80 .45
		Sound mailboxhit2 80 80 .45
		Sound mailboxhit3 80 80 .45

	End

End



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
		Geom	Broken_FX_Mailbox
		
		Bhvr	V_COV\PhysicsEnabled\MetalChunkScatter.bhvr
		BhvrOverride
			PositionOffset		0 .5 0
			StartJitter		0 0 0		
			scale			1.1 1.1 1.1

			InitialVelocity		0 0.4 0
			InitialVelocityJitter	0.25 0.25 0.25
			StartColor		100 90 80

			PyrRotateJitter		0 0 0
			spinJitter		.2 .3 .2

			Drag			0.025

			physics 		1
			physRadius 		0.8
			physGravity 		.8
			physRestitution 	0.1
			physSFriction 	 	0.3
			physDFriction 	 	0.5
			FadeOutLength		510

			PhysDebris		3
		End

		CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001

		Lifespan	11
	End

End

Condition
	On 	Time
	Time 	0
	Repeat	15
	RepeatJitter	10

	Event
		Type	start 
		At	chest
		
		Geom	mail_letter01
		Geom	mail_letter02
		Geom	mail_letter03
		
		BhvrOverride
			PositionOffset		0 2 0
			StartJitter		0 1 0
			
			InitialVelocity		.0 0.3 .0
			InitialVelocityJitter	0.5 0.5 0.5

			PyrRotateJitter		180 180 180
			SpinJitter		0.2 0.2 0.2

			Drag			0.025

			physics 		1
			physRadius 		3
			physGravity 		.5
			physRestitution 	0.15
			physSFriction 	 	0.7
			physDFriction 	 	0.7
			Gravity 		.02

			PhysDebris		1
			PhysDensity		3
			FadeOutLength		150
			PhysScale		1.25 2.5 1.25
		End

		CThresh 0.0001

		Lifespan	10
		
		#Part	:Letter.part

	End
End

Condition
	On 	Time
	Time 	0
	Repeat	10
	RepeatJitter	5

	Event
		HardwareOnly
		Type	start 
		At	chest
		
		Geom	mail_letter01
		Geom	mail_letter02
		Geom	mail_letter03
		
		BhvrOverride
			PositionOffset		0 2 0
			StartJitter		0 1 0
			
			InitialVelocity		.0 0.5 .0
			InitialVelocityJitter	0.75 0.75 0.75

			PyrRotateJitter		180 180 180
			SpinJitter		0.2 0.2 0.2

			Drag			0.025

			physics 		1
			physRadius 		3
			physGravity 		.5
			physRestitution 	0.15
			physSFriction 	 	0.7
			physDFriction 	 	0.7
			Gravity 		.02

			PhysDebris		1
			PhysDensity		3
			FadeOutLength		150
			PhysScale		1.25 2.5 1.25
		End

		CThresh 0.0001

		Lifespan	10
		
		#Part	:Letter.part

	End
End

Condition
	On 	Time
	Time 	0
	Repeat	15
	RepeatJitter	10

	Event
		Type	start 
		At	chest
		
		Geom	mail_letter01
		Geom	mail_letter02
		Geom	mail_letter03
		Geom	mail_letter04
		
		BhvrOverride
			PositionOffset		0 2 0
			StartJitter		0 1 0

			InitialVelocity		.0 0.4 .0
			InitialVelocityJitter	0.2 0.3 0.2

			PyrRotateJitter		180 180 180
			SpinJitter		0.2 0.2 0.2

			Drag			0.025

			physics 		1
			physRadius 		3
			physGravity 		.5
			physRestitution 	0.15
			physSFriction 	 	0.7
			physDFriction 	 	0.7
			Gravity 		.02
			PhysDebris		1
			PhysDensity		3
			
			FadeOutLength		150
			PhysScale		1.25 2.5 1.25
		End

		CThresh 0.0001

		Lifespan	10
		
		#Part	:Letter.part

	End
End

Condition
	On 	Time
	Time 	0
	Repeat	10
	RepeatJitter	5

	Event
		HardwareOnly
		Type	start 
		At	chest
		
		Geom	mail_letter01
		Geom	mail_letter02
		Geom	mail_letter03
		Geom	mail_letter04
		
		BhvrOverride
			PositionOffset		0 2 0
			StartJitter		0 1 0

			InitialVelocity		.0 0.25 .0
			InitialVelocityJitter	0.35 0.4 0.35

			PyrRotateJitter		180 180 180
			SpinJitter		0.2 0.2 0.2

			Drag			0.025

			physics 		1
			physRadius 		3
			physGravity 		.5
			physRestitution 	0.15
			physSFriction 	 	0.7
			physDFriction 	 	0.7
			Gravity 		.02
			PhysDebris		1
			PhysDensity		3
			
			FadeOutLength		150
			PhysScale		1.25 2.5 1.25
		End

		CThresh 0.0001

		Lifespan	10
		
		#Part	:Letter.part

	End
End

Condition
#	On 	Time
#	Time 	0
#	Repeat	19
#
#	Event
#		Type	start 
#		At	chest
#		
#		BhvrOverride
#			InitialVelocity		.0 0.5 .0
#			InitialVelocityJitter	0.6 0.5 0.6
#			PyrRotateJitter		180 180 180
#			Drag			0.02
#			physics 		1
#			physRadius 		0.15
#			physGravity 		.45
#			physRestitution 	0.05
#			physSFriction 	 	0.7
#			physDFriction 	 	0.7
#			Gravity 		.04
#		End
#
#		
#		Part	:WhiteLetterBack.part
#
#	End
#
#End
#
#Condition
#	On 	Time
#	Time 	0
#	Repeat	19
#
#	Event
#		Type	start 
#		At	chest
#		
#		BhvrOverride
#			InitialVelocity		.0 0.5 .0
#			InitialVelocityJitter	0.6 0.5 0.6
#			PyrRotateJitter		180 180 180
#			Drag			0.02
#			physics 		1
#			physRadius 		0.15
#			physGravity 		.45
#			physRestitution 	0.05
#			physSFriction 	 	0.7
#			physDFriction 	 	0.7
#			Gravity 		.04
#		End
#
#		
#		Part	:WhiteLetter.part
#	End
#
#End
#

End			