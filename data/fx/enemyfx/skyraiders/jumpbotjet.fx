#########################################################
##	Fireweapon
########################################################
FxInfo

Input  
	Inpname	Chest
End
 

Flags    InheritAlpha

########################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	local
		Type	local
		At	Chest
		Geom	jumpBotJet
	
	End

	
	Event
		EName 	JetL
		Type	local 
		At	local
		 AltPiv 1
		Part1	EnemyFX/5thColumn/Hoverbot/JetPackThrusterGlowBlue.part
		Part2	EnemyFX/5thColumn/Hoverbot/ThrusterCenterBlue.part
		Part3	EnemyFX/5thColumn/Hoverbot/Thrustercore.part
		Part4	EnemyFX/5thColumn/Hoverbot/ThrusterTrail.part
		Part5	EnemyFX/5thColumn/Hoverbot/ThrusterGases.part
		Part	EnemyFX/5thColumn/Hoverbot/ThrusterGasesTrail.part
		Sound jetpack2_loop 70 70 .3
	End
	
End

Condition
	On 	triggerbits
	Triggerbits	death
	
################ If you want the guy to turn invisible right as he dies AND play particles, do this on triggerbits death
################ (you can't set TicksToLingerAfterDeath to less than about 15 if you want to have a 
################ deathFx because he'll be dead before the fx happens.)
	
	Event
		EName 	Local
		Type	Destroy
	End

	Event
		EName 	JetL
		Type	Destroy
	End

End

End
##################################

