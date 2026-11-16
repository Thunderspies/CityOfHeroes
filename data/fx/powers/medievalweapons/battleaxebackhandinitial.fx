#########################################################
##	template

FxInfo

LifeSpan	155

Condition
	On 	Time
	Time 	18

	Event
		Type	local
		At	Origin
		Sound Whooshbig3 100.0 100.0 .8
	End

End


Condition
	On 	Time
	Time 	55

	Event
		EName	Streak
		Type	Start
		At	Mystic
		Geom	swoosh
		bhvr	behaviors/Medieval1.bhvr
		Sound Whoosh5 100.0 100.0 .6
		Lifespan 1
	End

End

##################################################################
##Projectile
###########################################################

Condition
	On 	Time
	Time 	57

	Event
		EName	Prime
		Type	start
		At	FootR

		Bhvr	behaviors/Focusprojectile.bhvr
		Part	:EnergyTrail.part

		Magnet	T_Root
		LookAt	T_Root

		
	End

End

Condition
	On 	Time
	Time 	51

	Event
		EName	Prime1
		Type	start
		At	FootR
		
		Part	:BattleAxeRaysTall.part
		Part	:BattleAxeRaysTallCenter.part
		Part	:BattleAxeRaysTall.part
		Part	:BattleAxeRaysTallCenter.part

		Bhvr	behaviors/Focusprojectile.bhvr
		

		Magnet	T_Root
		LookAt	T_Root

			
	End

End




Condition
	On 	Time
	Time 	44

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

Condition
	On 	Prime6Hit
	
	Event
		EName 	Prime6
		Type	Destroy

	End


End


Condition
	On 	Prime5Hit
	
	Event
		EName 	Prime5
		Type	Destroy

	End


End

Condition
	On 	Prime4Hit
	
	Event
		EName 	Prime4
		Type	Destroy

	End


End

End	

	