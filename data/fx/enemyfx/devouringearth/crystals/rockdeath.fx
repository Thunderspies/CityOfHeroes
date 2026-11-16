
#########################################################
##	
##
FxInfo

Input  
	InpName	CHEST
End

Input  
	InpName	Origin
End	
	

Condition
	On 	triggerbits
	Triggerbits	death
	
################ If you want the guy to turn invisible right as he dies AND play particles, do this on triggerbits death
################ (you can't set TicksToLingerAfterDeath to less than about 15 if you want to have a 
################ deathFx because he'll be dead before the fx happens.)
	Event
		EName 	Prime
		Type	Local
		At	Origin 
		Bhvr	behaviors/vanish.bhvr
		Flags	AdoptParentEntity
	End

	Event
		EName 	Prime
		Type	Destroy
	End
#################### End do this

	Event
		EName 	Mallet
		Type	start 
		At	Chest
		Bhvr	Behaviors/genericParticleFade.bhvr
		Part1	EnemyFX/DevouringEarth/RockGuyDeath/StalagtiteDustHit.part
		Part2	EnemyFX/DevouringEarth/RockGuyDeath/StalagtiteDustHit2.part
		Part3	EnemyFX/DevouringEarth/RockGuyDeath/WeaponRocks06.part
		Part4	EnemyFX/DevouringEarth/RockGuyDeath/BoulderBreak.part
		Part4	EnemyFX/DevouringEarth/RockGuyDeath/BoulderBreak2.part
		Part5	EnemyFX/DevouringEarth/RockGuyDeath/WeaponRocks07.part
		Sound stonehit2 100 100 .85
		
	End

End

End
