#########################################################
##	onfire
##
FxInfo  

LifeSpan	300


Condition
	Time 	0

		Event
		Type	local
		At	root
		
		BHVR	Behaviors/VaultCameraShake.bhvr
				
		LifeSpan	300
	End

	Event
		EName	VaultHookup
		Type	local
		At	root
		
		BHVR	Behaviors/GenericParticleFade.bhvr
		Geom	VaultHookup
		Sound VaultExplo2 90 90 .97
		
		LifeSpan	300
	End

#####################################################################################
##Sound   #
##############################################################
	
#	Event
#		EName 	Fire
#		Type	start
#		At	VaultHookup
#		Altpiv	4
#		#Sound	molotovhit1 80 30 .86
#		#Sound	fireexplo 80 30 .72
#		#Sound	firezap 80 30 .76
#		#Sound	molotovhit1 80 30 .86
#		LifeSpan 20
#			
#	End

###################################################################

	Event
		EName 	Prime1
		Type	Local 
		At	VaultHookup
		Altpiv	4
		
		Part	:InfernoCare.part
		Part	:Inferno.part
		Part	:Sparks.part
		
		LifeSpan	200

	End

	Event
		EName 	Prime2
		Type	Local 
		At	VaultHookup
		Altpiv	4
		Part	:InfernoExplosion.part
		Part	:SparksSmall.part
		
		LifeSpan	280

	End

End

Condition
	On 	Time
	Time 	3

	Event
		EName 	Prime
		Type	Local 
		At	VaultHookup
		Altpiv	4
		Part1	Generic\InfernorockBits.part
		Part2	Generic\InfernorockBitz.part
		#Sound	FireExplo 100.0 30.0 .8	
		LifeSpan	200
	End

End

Condition
	Time 	3
	Chance	1

	Event
		Type	start
		At	VaultHookup
		Altpiv	4

		Bhvr	:Phys1.bhvr
		Part1	powers/firecontrol/fire.part
		Part1	powers/firecontrol/fire.part
		Lifespan	60
	End

End
Condition
	Time 	3
	Chance	1

	Event
		Type	start
		At	VaultHookup
		Altpiv	4

		Bhvr	:Phys1.bhvr
		Part1	powers/firecontrol/fire.part
		Part1	powers/firecontrol/fire.part
		Lifespan	60
	End

End
Condition
	Time 	3
	Chance	1

	Event
		Type	start
		At	VaultHookup
		Altpiv	4

		Bhvr	:Phys1.bhvr
		Part1	powers/firecontrol/fire.part
		Part1	powers/firecontrol/fire.part
		Lifespan	60
	End

End
Condition
	Time 	3
	Chance	1

	Event
		Type	start
		At	VaultHookup
		Altpiv	4

		Bhvr	:Phys1.bhvr
		Part1	powers/firecontrol/fire.part
		Part1	powers/firecontrol/fire.part
		Lifespan	60
	End

End
Condition
	Time 	3
	Chance	1

	Event
		Type	start
		At	VaultHookup
		Altpiv	4

		Bhvr	:Phys1.bhvr
		Part1	powers/firecontrol/fire.part
		Part1	powers/firecontrol/fire.part
		Lifespan	60
	End

End
Condition
	Time 	3
	Chance	1

	Event
		Type	start
		At	VaultHookup
		Altpiv	4

		Bhvr	:Phys1.bhvr
		Part1	powers/firecontrol/fire.part
		Part1	powers/firecontrol/fire.part
		Lifespan	60
	End

End
Condition
	Time 	3
	Chance	1

	Event
		Type	start
		At	VaultHookup
		Altpiv	4

		Bhvr	:Phys1.bhvr
		Part1	powers/firecontrol/fire.part
		Part1	powers/firecontrol/fire.part
		Lifespan	60
	End

End
Condition
	Time 	3
	Chance	1

	Event
		Type	start
		At	VaultHookup
		Altpiv	4

		Bhvr	:Phys1.bhvr
		Part1	powers/firecontrol/fire.part
		Part1	powers/firecontrol/fire.part
		Lifespan	60
	End

End
Condition
	Time 	3
	Chance	1

	Event
		Type	start
		At	VaultHookup
		Altpiv	4

		Bhvr	:Phys1.bhvr
		Part1	powers/firecontrol/fire.part
		Part1	powers/firecontrol/fire.part
		Lifespan	60
	End

End
Condition
	Time 	3
	Chance	1

	Event
		Type	start
		At	VaultHookup
		Altpiv	4

		Bhvr	:Phys1.bhvr
		Part1	powers/firecontrol/fire.part
		Part1	powers/firecontrol/fire.part
		Lifespan	60
	End

End
Condition
	Time 	3
	Chance	1

	Event
		Type	start
		At	VaultHookup
		Altpiv	4

		Bhvr	:Phys1.bhvr
		Part1	powers/firecontrol/fire.part
		Part1	powers/firecontrol/fire.part
		Lifespan	60
	End

End
Condition
	Time 	3
	Chance	1

	Event
		Type	start
		At	VaultHookup
		Altpiv	4

		Bhvr	:Phys1.bhvr
		Part1	powers/firecontrol/fire.part
		Part1	powers/firecontrol/fire.part
		Lifespan	60
	End

End
Condition
	Time 	3
	Chance	1

	Event
		Type	start
		At	VaultHookup
		Altpiv	4

		Bhvr	:Phys1.bhvr
		Part1	powers/firecontrol/fire.part
		Part1	powers/firecontrol/fire.part
		Lifespan	60
	End

End
Condition
	Time 	3
	Chance	1

	Event
		Type	start
		At	VaultHookup
		Altpiv	4

		Bhvr	:Phys1.bhvr
		Part1	powers/firecontrol/fire.part
		Part1	powers/firecontrol/fire.part
		Lifespan	60
	End

End
Condition
	Time 	3
	Chance	1

	Event
		Type	start
		At	VaultHookup
		Altpiv	4

		Bhvr	:Phys1.bhvr
		Part1	powers/firecontrol/fire.part
		Part1	powers/firecontrol/fire.part
		Lifespan	60
	End

End
Condition
	Time 	3
	Chance	1

	Event
		Type	start
		At	VaultHookup
		Altpiv	4

		Bhvr	:Phys1.bhvr
		Part1	powers/firecontrol/fire.part
		Part1	powers/firecontrol/fire.part
		Lifespan	60
	End

End

######################################################################################################



Condition
	Time 	0
	Chance	1

	Event
		Type	start
		At	VaultHookup
		Altpiv	1

		Geom	Vault_Debris01
		Bhvr 	:VaultPhys.bhvr
		CEvent 	:FireBallSparks.fx
		CThresh 0.1

		Lifespan	200
	End

End

Condition
	Time 	0
	Chance	1

	Event
		Type	start
		At	VaultHookup
		Altpiv	2

		Geom	Vault_Debris02
		Bhvr 	:VaultPhys.bhvr
		CEvent 	:FireBallSparks.fx
		CThresh 0.1

		Lifespan	200
	End

End

Condition
	Time 	0
	Chance	1

	Event
		Type	start
		At	VaultHookup
		Altpiv	3

		Geom	Vault_Debris03
		Bhvr 	:VaultPhys.bhvr
		CEvent 	:FireBallSparks.fx
		CThresh 0.1

		Lifespan	200
	End

End

Condition
	Time 	0
	Chance	1

	Event
		Type	start
		At	VaultHookup
		Altpiv	4

		Geom	Vault_Debris04
		Bhvr 	:VaultPhys.bhvr
		CEvent 	:FireBallSparks.fx
		CThresh 0.1

		Lifespan	200
	End

End

Condition
	Time 	0
	Chance	1

	Event
		Type	start
		At	VaultHookup
		Altpiv	5

		Geom	Vault_Debris05
		Bhvr 	:VaultPhys.bhvr
		CEvent 	:FireBallSparks.fx
		CThresh 0.1

		Lifespan	200
	End

End

Condition
	Time 	0
	Chance	1

	Event
		Type	start
		At	VaultHookup
		Altpiv	6

		Geom	Vault_Debris06
		Bhvr 	:VaultPhys2.bhvr
		CEvent 	:FireBallSparks.fx
		CThresh 0.1

		Lifespan	200
	End

End

Condition
	Time 	0
	Chance	1

	Event
		Type	start
		At	VaultHookup
		Altpiv	7

		Geom	Vault_Debris07
		Bhvr 	:VaultPhys.bhvr
		CEvent 	:FireBallSparks.fx
		CThresh 0.1

		Lifespan	200
	End

End

Condition
	On 	Time
	Time	9

	Event
		EName 	Prime1
		Type	Destroy
	End

End

Condition
	On 	Time
	Time	3

	Event
		EName 	Prime2
		Type	Destroy
	End

End


Condition
	On 	Time
	Time	7

	Event
		EName 	Prime
		Type	Destroy
	End

End	

Condition
	On 	Time
	Time 	30

	Event
		EName 	RadiusFxScale
		Type	Destroy 
	End


End

Condition
	On 	Time
	Time 	300

	Event
		EName 	All
		Type	Destroy 
	End


End


End			
