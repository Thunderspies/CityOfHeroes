#########################################################
##	FireRing
##
FxInfo

Flags		InheritAlpha	

####################
###Lightning Hit
##################################

#Condition
#	On 	cycle
#	Time 	7
#	Chance	.2
#
#	Event
#		EName 	Rings
#		Type	Local 
#		At	root
#		
#		BhvrOverride
#			PositionOffset	0 20 0
#		End
#
#		Part	:LightningArch_Branches.part
#		Part	:LightningArch_Branches_White.part
#		LifeSpan	5
#
#	End
#
#	Event
#		EName 	Rings
#		Type	Local 
#		At	root
#		
#		ChildFx	POWERS\ElectricityControl\Red_DustDevil_Lightning.fx
#		#childfx	:Tornado_LightningArch_Branches.fx
#		
#		Lifespan 7
#	End
#
####################
###Phs Hit
##################################
#	Event
#
#		Type	start
#		At	Root
#
#		BhvrOverride
#			
#			physics 		1
#			physRadius 		0.8
#			physGravity 		.9
#			physRestitution 	0.5
#			physSFriction 	 	0.3
#			physDFriction 	 	0.5
#
#
#		End
#
#		CEvent	POWERS\ElectricityControl\LightningRodHit2.fx
#		Cdestroy	1
#		CThresh		0.001
#
#		LifeSpan	50
#		
#	End
#
#End

Condition

	On	Time
	Time	0
	Event
		Type	Local
		At	origin
		Sound spellcast1 70 70 .7
	End

End


Condition
	On 	Time
	Time 	20

	Event
		EName 	Ring1
		Type	local 
		At	root
	
		Part	:Steam.part
		Part	:Mist.part
		Part	:Rocks.part
		Part	:Sand.part
		Part	:BlackRocks.part
		Part	:Dust.part
		Part	:Dust2.part
		Part	:BlackDust2.part
		Part	:DustTrail.part
		Part	:BlackCoreDust.part
		Part	:BlackCore.Part


		bhvr	behaviors/SuperSpeedWirlwind.bhvr	
		#Sound	Whirlwind2_loop 80 10 .8

		
	End

	Event
		EName 	Ring1
		Type	local 
		At	root
	
		Part	:BlackDust.part
		Part	:BlackSand.part
		Part	:EnsnareCore.part
		Part	:SmokeTrailCore.part

				#Delete# Part3	:EnsnareMistCore.part

		Part	:rocksCore.part
		Part	:BlackrocksCore.part
		Part	:EnsnareMist2Core.part
		bhvr	behaviors/SuperSpeedWirlwind.bhvr	
		Sound Whirlwind2_loop 80 80 .8

		
	End
		
	Event
		Ename	SwirlForce1
		Type	Local
		At	Root
		
		BhvrOverride
			PositionOffset		0 6 0
			PhysForceType		CCWSwirl
			PhysForceRadius		12
			PhysForceCentripetal	.65
			PhysForcePower		100
			PhysForcePowerJitter	5
		End

		#Lifespan 40

	End
			
	Event
		Ename	SwirlForce1
		Type	Local
		At	Root
		
		BhvrOverride
			PositionOffset		0 20 0
			PhysForceType		CCWSwirl
			PhysForceRadius		15
			PhysForceCentripetal	1
			PhysForcePower		100
			PhysForcePowerJitter	5
		End

		#Lifespan 40

	End

	Event
		Ename	LevitationForce1
		Type	Local
		At	Root
		
		BhvrOverride
			PhysForceType		Up
			PhysForceRadius		15
			PhysForcePower		100
			PhysForcePowerJitter	5
		End

		#Lifespan	30

	End

	
	Event
		Ename	LevitationForce2
		Type	Local
		At	Root
		
		BhvrOverride
			PositionOffset		0 15 0
			PhysForceType		Up
			PhysForceRadius		10
			PhysForcePower		50
			PhysForcePowerJitter	5
		End

		#Lifespan	30

	End

	
	Event
		Ename	SpreadForce
		Type	Local
		At	Root
		
		BhvrOverride
			PositionOffset		0 0 0
			PhysForceType		Out
			PhysForceRadius		15
			PhysForcePower		25
			PhysForcePowerJitter	5
		End

		#Lifespan	30

	End

		
	Event
		Ename	Shhot
		Type	Local
		At	Root
		
		BhvrOverride
			PositionOffset		0 20 0
			PhysForceType		Out
			PhysForceRadius		5
			PhysForcePower		300
			PhysForcePowerJitter	5
		End

		#Lifespan	30

	End

End

Condition
	On 	cycle
	Time 	8

	Event
		EName 	Rings
		Type	Local 
		At	root
		
		Bhvr 	behaviors/DustDevilMotion.bhvr
		geom	DustDevilWisp	#PsionicWisp	
		
		Lifespan 5
	End

End

Condition
	On 	cycle
	Time 	12

	Event
		EName 	Rings
		Type	Local 
		At	root
		
		Bhvr 	behaviors/DustDevilDark.bhvr
		geom	DustDevilWisp	#PsionicWisp	
		
		Lifespan 15
	End

End

Condition
	On 	cycle
	Time 	10

	Event
		EName 	Rings
		Type	Local 
		At	root
		
		Bhvr 	behaviors/DustDevilMedium.bhvr
		geom	DustDevilWisp	#PsionicWisp	
		
		Lifespan 50
	End

End

Condition
	On 	Time
	Time 	64

	Event
		Type	local 
		At	root


		
		Bhvr 	behaviors/DustDevilCore.bhvr
		geom	DustDevilFunnel	
		
	End

End


End
################################################################
		