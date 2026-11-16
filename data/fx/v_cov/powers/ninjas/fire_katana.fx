#########################################################
##	jack
##

FxInfo

Input  
	InpName	WepR
End

Flags    InheritAlpha

Condition
	On 	Time
	Time 	9

	Event
#		EName 	katanaHookup
		Type	Local 
		At	WepR
		Geom	FireKatana
		Sound swordshing 100 100 .7
		Bhvr	Behaviors/Weapon_Fade_UP.bhvr
	End
	
	Event
		EName 	katanaHookup
		Type	Local 
		At	WepR
		Geom	FireKatanaHandle
		Bhvr	Behaviors/Weapon_Fade_UP.bhvr
	End

	Event
		EName 	SwordSpot2
		Type	Local
		At	katanaHookup
		 AltPiv 1
		
		Part1	:fireweapon3follow.part
		Part2	:fireweapon2.part
		Part3	Powers/FireControl/fireweaponspark.part
		
		BHVR	Behaviors/GenericParticleFade.Bhvr

		POther	SwordSpot1
		PMagnet	SwordSpot1
	End
	

	Event
		EName 	SwordSpot1
		Type	Local 
		At	katanaHookup
		 AltPiv 1

		Part1	:fireweapon1.part
		Part2	:fireweapon2.part
		Part3	:fireweapon3.part
		Part4	:fireweaponWhitness.part
		POther	SwordSpot2
		PMagnet	SwordSpot2
		BHVR	Behaviors/OffsetGenericParticleFade.Bhvr
	End

	Event
		EName 	SwordSpot3
		Type	Local
		At	katanaHookup
		 AltPiv 1
		Part1	Powers/FireControl/fireweaponspark.part
		Part2	:fireweapon2.part

		BHVR	Behaviors/GenericParticleFade.Bhvr
		
	End

End

End		