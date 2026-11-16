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
	InpName	WepL
End


##################################
Condition
	On 	Time
	Time 	29

	

	
	Event
		EName 	Prime
		Type	start
		At	WepL
		Bhvr	Behaviors\WristCrossbowBoltProjectile.bhvr
		Geom	CrossBowBolt
		Sound Crossbow1 100.0 100.0 .7

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