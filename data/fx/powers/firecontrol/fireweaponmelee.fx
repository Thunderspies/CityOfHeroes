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
		EName 	Prime
		Type	Local 
		At	WepR
		Bhvr	behaviors/powers/coldcontrol/IceWeapon.bhvr
		Geom	FX_FireWeapon
	
	End

	Event
		EName 	SwordSpot1
		Type	Local
		At	Prime
		 AltPiv 1
		
		Part1	:fireweapon3follow.part
		Part2	:fireweapon2.part
		Part3	:fireweaponspark.part

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

End
##################################

