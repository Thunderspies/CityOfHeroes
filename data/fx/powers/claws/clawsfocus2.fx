#########################################################
##	Fireweapon
########################################################
FxInfo

Input  
	Inpname	Origin
End

Input  
	Inpname	Mystic
End

Input  
	Inpname	wepR
End

Input  
	Inpname	Chest
End

Input  
	Inpname	Target
End

Input  
	Inpname	root
End

LifeSpan	100

########################################################################


Condition
	On 	Time
	Time 	19

	Event
		EName	roothookup
		Type	local
		At	root
		Bhvr	behaviors/alpha.bhvr
		Geom	RootToChestAdjustment

				
	End
	
	Event
		EName	hookup
		Type	local
		At	roothookup
		altpiv  1
		Geom	LungeNode

				
	End
	
	Event
		EName	Prime
		Type	start
		At	hookup
		Geom	onset
		altpiv	1

		Bhvr	behaviors/ClawspinPrjct.bhvr
		Part1	:ClawEnergyRings.part

		Magnet	Target
		LookAt	Target

		
	End

	Event
		EName	Prime1
		Type	start
		At	hookup
		altpiv	2
		Geom	offset

		Bhvr	behaviors/ClawspinPrjct2.bhvr
		Part1	:ClawEnergyRings.part

		Magnet	Target
		LookAt	Target

			
	End

End


Condition
	On 	Time
	Time 	14.5

	Event
		EName	Streak
		Type	local
		At	Mystic
		Geom	swoosh
		bhvr	behaviors/Claws.bhvr
		Sound clawtwirl5 100.0 100.0 .7
	End

End


Condition
	On 	Time
	Time 	17.5
	
	Event
		EName	streak
		Type	Destroy
		
	End
	
End

Condition
	On 	PrimeHit

	Event
		EName 	Prime
		Type	Destroy

	End

End

Condition
	On 	Prime1Hit
	
	Event
		EName 	Prime1
		Type	Destroy

	End


End

End
##################################

