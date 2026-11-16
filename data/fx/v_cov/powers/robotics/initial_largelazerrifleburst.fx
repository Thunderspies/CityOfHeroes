#########################################################
##	
##

FxInfo

LifeSpan  300

##################################
Condition
	On 	Time
	Time 	41

	Event

		Type	local
		At	origin
		Sound laserRifleBurst 80 80 .88
	End

End

Condition
	On 	Time
	Time 	41

	
	Event
		ENAME   p1
		Type	Posit 
		At	t_chest
		Geom	Centerdummy
		Lookat	origin
		#Sound	laserRifleBurst 80 70 .88
	End

	Event
		ENAME   p1offset
		Type	local 
		At	p1
		altpiv	1
		bhvr	:LazerOffset.bhvr
	End

	Event
		ENAME   p1a
		Type	Local 
		At	WepR
		Geom	newrobotics
		#Geom	Mastermind_Military_Rifle
	End

	Event
		Type	Local
		At	p1a
		Altpiv 2
#		Geom	Fx_iceweapon
#		BHVR	:BlastOffset.bhvr
		Part	:BlastOriginStar.part
		Part	:BlastOriginEnergy.part
		Part	:BlastOriginEnergy.part
		LifeSpan	5
	End

End

Condition
	On 	Time
	Time 	43



	Event
		EName 	Prime
		Type	Start
		At	p1a
		Altpiv 2
		
		Part	:LargeVillianSniper_Head.part
		Part	:LargeVillianSniper_HeadCore.part

		Part	:VillianSniper_Head.part
		Part	:VillianSniper_HeadCore.part
		

		BHVR	:VillianSniperlTracer.bhvr

		Magnet	P1
		Lookat	P1
	End
	
End

##################################


Condition
	On	PrimeHit
	
	Event
		Ename	Prime
		Type	Destroy
	End

End

End				