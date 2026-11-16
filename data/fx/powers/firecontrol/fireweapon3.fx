#########################################################
##	Fireweapon
########################################################
FxInfo

Input  
	Inpname	WepR
End




	
Condition
	On 	Time
	Time 	0

	Event
		EName	SwordFlash
		Type	local
		At	WepR
		Part1	:FireHandsFlash.part
		BHVR	Behaviors/GenericParticleFade.Bhvr
		
	End

	Event
		EName	SwordFlash
		Type	local
		At	WepR
		Sound glow3_loop 80 80 .33
	End

	Event
		EName 	Prime
		Type	Local 
		At	WepR
		Bhvr	behaviors/FireWeapon.bhvr
		Geom	FX_FireWeaponGreater
		#Sound	FireSword6 70.0 30.0 .88
	
	End

	Event
		EName 	SwordSpot1
		Type	Local
		At	Prime
		 AltPiv 1
		Sound FireSword_loop 60.0 60.0 .33
		Part1	:fireweapon3followa.part
		Part2	:fireweapon2a.part
		Part3	:fireweaponspark.part


		POther	SwordSpot2
		PMagnet	SwordSpot2
	End
	

	Event
		EName 	SwordSpot2
		Type	Local 
		At	Prime
		 AltPiv 2

		Part1	:fireweapon1a.part
		Part2	:fireweapon2a.part
		Part3	:fireweapon3a.part
		#Part4	:fireweaponWhitness.part
		Part5	:fireweaponBlueGlow.part
		POther	SwordSpot1
		PMagnet	SwordSpot1
	End

	Event
		EName 	SwordSpot3
		Type	Local
		At	WepR
		Part1	:fireweaponspark.part
		Part2	:fireweapon2a.part
	End

End

Condition
	On 	Time
	Time	2
	
	Event	
		EName	SwordFlash
		Type	local
		At	WepR
		Sound Firestreaks3 80.0 80.0 .6
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

