#########################################################
##	cot_waterfall_med.fx
##
FxInfo

#LifeSpan	10

###########################################################

Condition
	On	Time
	Time	0
	
	Event
		EName	Device
		Type	local
		At	mystic
		Sound PCM_telemetry 60 60 .2
		Bhvr	:Scale.bhvr

		Geom	PCM_Device
		LifeSpan	797
	End

	Event

		Type	local
		At	Device
		Altpiv	1
		

		Part	:DaemonPupil.part
		LifeSpan	341

	End

	Event

		Type	local
		At	Device
		Altpiv	2
		
		
		Part	:DaemonPupil.part
		LifeSpan	341

	End


	Event

		Type	local
		At	Device
		Altpiv	3
		
		
		Part	:DaemonPupil.part
		LifeSpan	341

	End


	Event

		Type	local
		At	Device
		Altpiv	4
		
		
		Part	:DaemonPupil.part
		LifeSpan	341

	End


	Event

		Type	local
		At	Device
		Altpiv	5
		
		
		Part	:DaemonPupil.part
		LifeSpan	341

	End


	Event

		Type	local
		At	Device
		Altpiv	6
		
		Part	:DaemonPupil.part
		LifeSpan	341

	End


	Event

		Type	local
		At	Device
		Altpiv	7
		
		Part	:DaemonPupil.part
		LifeSpan	341

	End


	Event

		Type	local
		At	Device
		Altpiv	8
		
		Part	:DaemonPupil.part
		LifeSpan	341

	End


	Event

		Type	local
		At	Device
		Altpiv	9
		
		Part	:DaemonPupil.part
		LifeSpan	341

	End


	Event

		Type	local
		At	Device
		Altpiv	10
		
		Part	:DaemonPupil.part
		LifeSpan	341

	End

End

##################################################################################
############################Psionic energy########################################

Condition
	On	Time
	Time	230

	Event
		EName	core
		Type	Local
		At	head
		Geom	PsionicRigging		
		#bhvr	behaviors/PsionicsScale.bhvr
		Sound psionic3 80 80 .9
		LifeSpan	60
	End

	Event
		EName	core2
		Type	Local
		At	core
		  Altpiv   1
		geom	EyeBeams02
		bhvr	behaviors/PsionicsScale2.bhvr
		LifeSpan	60
	End

	Event
		EName	core3
		Type	Local
		At	core
		   Altpiv   3
		geom	EyeBeams01
		bhvr	behaviors/PsionicsScale.bhvr
		LifeSpan	60
	End

	Event
		EName	RingsAndParticles
		Type	Local
		At	core
		altpiv	2
		
		Part1	Powers/Psionics/PsionicBubbles.part
		Part2	Powers/Psionics/PsiGlow.part
		LifeSpan	60
	End

	Event
		EName	RingsAndParticles1
		Type	Local
		At	core
		altpiv	1
		
		Part1	Powers/Psionics/EnergyCoreLocal.part
		LifeSpan	60
		
	End

	Event
		EName	RingsAndParticles2
		Type	Local
		At	core
		altpiv	3
		
		Part1	Powers/Psionics/EnergyCoreLocal.part
		LifeSpan	60
		
	End

	Event
		EName	RingsAndParticles3
		Type	Local
		At	core
		altpiv	2
		
		Part1	Powers/Psionics/EnergyCore.part
		LifeSpan	60
		
	End

End
	
###########################################################################

Condition
	On	Time
	Time	305	#301

	Event
		EName	faceringsRigging
		Type	Posit
		At	mystic
		Part1	:InfuseLight2.part
		Part2	:Infuseglow.part
		Part3	:InfuseRings.part
		Part	:Temperalglow.part
		Part	:TemperalEnergyVeins.part
		Part1	:InfuseLight2.part
		Part2	:Infuseglow.part
		Part3	:Defuseglow.part
		#Sound	Infuse4 80 33 .6
		LifeSpan	40	#20
	End

	Event
		EName	facerings
		Type	Posit
		At	mystic
		
		Part4	:QuestionMark.part
		Part5	:NumberSign.part
		Part4	:PercentSign.part
		Part5	:Asterick.part
		Part1	:PsionicBubblesHit.part
		LifeSpan	50	#30
	End

	Event
		EName	facerings2
		Type	Posit
		At	mystic
		
		
		Part3	:TelekineticRingBig.part
		Part4	:HitStreaksBig.part
		LifeSpan	30	#10
	End

	Event
		
		Type	Posit
		At	mystic
		Part2	:HitRaysBig.part
		lifespan	30	#10
	End
	
End

Condition
	On	Time
	Time	313	#308

	Event
		Type	Posit
		At	mystic
		Sound Seer2 80 80 .77
		Part1	:TelekineticRingBig2.part
		LifeSpan	10
	End

End

##################################################################
##Second Stage
#################################################

Condition
	On	Time
	Time	341	#358

	Event
		
		Type	start
		At	mystic
		Sound PCM_explo 300 300 .93
		Bhvr	Behaviors/PCM_CameraShake.bhvr
		lifespan	220	#10
	End

	Event
		
		Type	start
		At	mystic
	
		Part	:TelekineticThinRingFast.part
		Part2	:SeconStageLightRaysBig.part
		lifespan	10	#10
	End

	Event
		
		Type	start
		At	mystic
	
		Part	:WhiteCloud.part
		lifespan	60	#10
	End

	Event
		
		Type	start
		At	Root
	
		Part	:WhiteCloudFlat.part
		lifespan	60	#10
	End
End

Condition
	On	Time
	Time	400

	Event
		
		Type	posit
		At	mystic
		Sound sizzle3 50 50 .18
		Part	:PCM_Steam.part
		lifespan	370	
	End
End	



Condition
	On	Time
	Time	351	#368

	Event
		
		Type	start
		At	mystic
	
		Part	:SeconStageQuickMovingGlows.part
		Part	:SeconStageQuickMovingGlows2.part
		lifespan	100	#10
	End
End	

Condition
	
	On	Time
	time	797

	Event
		EName	PHSDevice
		Type	start	#Create
#		Inherit	Position SuperRotation Scale
		At	mystic
		Sound whoosh5 50 50 .18
#		Bhvr	:Scale.bhvr

		Bhvr	V_COV/PhysicsEnabled/SparkOUT.bhvr

		BhvrOverride
			physics 		1
#			PhysScale		2.5 3.5 3.5
			Scale			1.6 1.6 1.6
			Spinjitter		.25 .25 .25
			InitialVelocity		0.0 0.55 -0.4	
#			Drag			.05
			InitialVelocityJitter	0.15 0.15 0.15
			physRestitution 	0.5
			physSFriction 	 	0.5
			physDFriction 	 	0.5
			PhysGravity		1
			Gravity			.1
		End

#		Part	:PCM_Steam2.part
		Geom	PCM_Device
		LifeSpan	80
		
		CThresh 	.1
		CDestroy	1
		Cevent		:PCM_Pieces.fx
	End

#	Event
#		Type	posit
#		At	PHSDevice		
#
#		Part	:PCM_Steam2.part
#		LifeSpan	81
#	End

End

End