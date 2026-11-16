#############################################################
## Test_Child_RockScatter1.fx
#############################################################

FXInfo

Lifespan 400

#############################################################

Condition
	On 	Time
	Time 	5
	#Repeat	10




#	Event
#		Type	start
#		At	Origin
#		Bhvr	V_COV\PhysicsEnabled\Test_Icecubes.bhvr
#		BhvrOverride
#			Scale			1 1 1
#			PositionOffset		10 1 0
#			StartJitter		0 2 0
#			StartColor		255 240 192
#			Gravity			0.05
#			physGravity		1.2
#			physRestitution 	0.5
#			physSFriction 	 	0.99
#			physDFriction 	 	0.9
#		End
#		Geom	Earth_FXDebris01A
#		Geom	Earth_FXDebris02A
#		Geom	Earth_FXDebris03A
#		Geom	Earth_FXDebris04A
#		Geom	Earth_FXDebris05A
#		Lifespan 300
#		LifespanJitter	90
#		CThresh 	0.00001
#	End
	Event
		Ename 	RockANchor
		Type	start
		At	Origin
		Bhvr	V_COV\PhysicsEnabled\Icecubes.bhvr
		BhvrOverride
			Scale			1 1 1
			PositionOffset		0 1 0
#			PositionOffsetJitter	10 1 10
			StartJitter		10 2 10
			StartColor		128 128 128
			Gravity			0.05
			physGravity		1.2
			physRestitution 	0.5
			physSFriction 	 	0.99
			physDFriction 	 	0.9
		End

		Geom	Earth_FXDebris01A
		Geom	Earth_FXDebris02A
		Geom	Earth_FXDebris03A
		Geom	Earth_FXDebris04A
		Geom	Earth_FXDebris05A

		Part	:FusionPulse_Attack_StoneSmoke.part

		Lifespan 50
		LifespanJitter	5
		CThresh 	0.00001
	End


#	Event
#		Type	Local
#		At	RockANchor
#		Part	:Test_StoneSmoke.part
#		Lifespan 30
#		LifespanJitter	0
#	#	CThresh 	0.00001
#	End
End

#Condition
#	On 	Time
#	Time 	3
#	Repeat	7
#
#	Event
#		Type	start
#		At	Origin
#		Bhvr	V_COV\PhysicsEnabled\Icecubes.bhvr
#		BhvrOverride
#			Scale			1 1 1
#			PositionOffset		0 1 0
#			StartJitter		0 2 0
#			StartColor		160 160 160
#			Gravity			0.05
#			physGravity		1.2
#			physRestitution 	0.5
#			physSFriction 	 	0.99
#			physDFriction 	 	0.9
#		End
#		Geom	Earth_FXDebris01A
#		Geom	Earth_FXDebris02A
#		Geom	Earth_FXDebris03A
#		Geom	Earth_FXDebris04A
#		Geom	Earth_FXDebris05A
#		Lifespan 300
#		LifespanJitter	90
#		CThresh 	0.00001
#	End
#End

#############################################################

End