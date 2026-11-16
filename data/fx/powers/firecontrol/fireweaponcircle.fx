#########################################################
##	Fireweapon
########################################################
FxInfo

Input  
	Inpname	WepR
End

Input  
	Inpname	Root
End

#Condition
#	On 	Time
#	Time 	83

#	Event
#		EName 	SwordSpot3
#		Type	local
#		At	root
#		Part1	:Swordflash.part
		
#	End

#	Event
#		EName 	Prime1
#		Type	Local 
#		At	root
#		bhvr    behaviors/powers/firecontrol/Infernobubble.bhvr
#		Part1	powers/firecontrol/swordFlame01.part
#		Part2	powers/firecontrol/SwordFlame02.part
		
#	End

#End

	
Condition
	On 	Time
	Time 	0

	Event
		EName	SwordFlash
		Type	local
		At	WepR
		bhvr    behaviors/GenericParticleFade.bhvr
		Part1	:FireHandsFlash.part
		
	
	End


	Event
		EName 	Prime
		Type	Local 
		At	WepR
		Bhvr	behaviors/FireWeapon.bhvr
		Geom	FX_FireWeapon
		Sound FireSword4 60 60 .88
	
	End

	Event
		EName 	SwordSpot1
		Type	Local
		At	Prime
		 AltPiv 1
		
		Part1	:fireweapon3follow.part
		Part2	:fireweapon2.part
		Part3	:fireweaponspark.part
		Sound FireSword_loop 60.0 60.0 .33
		POther	SwordSpot2
		PMagnet	SwordSpot2
	End
	

	Event
		EName 	SwordSpot2
		Type	Local 
		At	Prime
		 AltPiv 2

		Part1	:fireweapon1.part
		Part2	:fireweapon2.part
		Part3	:fireweapon3.part
		Part4	:fireweaponWhitness.part
		POther	SwordSpot1
		PMagnet	SwordSpot1
	End

Event
		EName 	SwordSpot3
		Type	Local
		At	WepR
		Part1	:fireweaponspark.part
		Part2	:fireweapon2.part
	End
End

Condition
	On 	Time
	Time 	5

	Event
		EName	SwordFlash
		Type	Destroy
		
	
	End
End


End
##################################

