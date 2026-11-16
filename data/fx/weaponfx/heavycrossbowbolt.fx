#########################################################
##	
##

FxInfo

LifeSpan  190

Input  
	InpName	Origin
End

Input  
	InpName	Target
End

Input  
	InpName	WepR
End


##################################
Condition
	On 	Time
	Time 	29

	
	Event
		EName 	x
		Type	start
		At	WepR
		Geom	Pistol
				
	End
	
	Event
		EName 	Prime
		Type	start
		At	x
		 Altpiv	1
		Bhvr	Behaviors\projectile2.bhvr
		Geom	CrossBowBolt
		Sound Crossbow1 100.0 100.0 .7
		Part1	:HeavyCrossbowHands2.part
		Part2	:HeavyCrossbowBoltTail2.part
		Part3	:HeavyCrossbowBoltTail.part
		#Part4	:HeavyCrossbowSparkTrail.part
		Part5	:HeavyCrossbowHands2.part

		Magnet	Target
		LookAt	Target
	End

End

##################################


Condition
	On 	PrimeHit

	Event
		Ename 	All
		Type	Destroy
	End
End


End				