#########################################################
##	
##

FxInfo

###LifeSpan  


Flags InheritAnimScale



#####  Sound  ############################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	Hips
		Sound BeepWarning_loop 40 40 .56
	End

End

Condition
	On 	Time
	Time 	0


#	Event
#		Type		Local 
#		At 		UlegL
#		ChildFx		World/Ruins/StreetSparks.fx
#	
#		
#	End




	Event
		Type		Local 
		At		Col_R
		ChildFx		World/Ruins/StreetSparks.fx

		
	End


End	



#######  flames  ####################################


Condition
	On 	time
	Time 	0
	Repeat  63

	Event 
		EName 	Sparkz
		Type 	Local
		At 	Chest
		ChildFX POWERS\FireControl\SmallBurst2.fx
		Lifespan 70
		LifespanJitter 	20
	End
End

Condition
	On 	Time
	Time 	0

	Event
		EName 	Fire
		Type	Local
		At	Neck

		Part1	POWERS\FireControl\FireBallTargetSteam.part
		Part2	POWERS\FireControl\FireBallTargetFlame2.part
		Part3	POWERS\FireControl\FireBallSpark.part
		Sound FireExplo 100.0 100.0 .8
	End

	Event
		EName 	flash
		Type	Local
		At	Chest
		Part1	powers/firecontrol/FireWeaponExplosion.part
		Part2	powers/firecontrol/FireWeaponHitSpark.part
	End

End

Condition
	On 	Time
	Time 	6
	Event
		EName 	flash
		Type	Destroy 

	End


End



End			
