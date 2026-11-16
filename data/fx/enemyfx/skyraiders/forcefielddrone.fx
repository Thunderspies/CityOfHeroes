#########################################################
##	Fireweapon
########################################################
FxInfo

Input  
	InpName	Root
End

#LifeSpan	70 

Flags    InheritAlpha


######################################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	infrontadj
		Type	Local
		At	Hips		
		Geom	RootToChestAdjustment
		Sound Thruster_loop 86 86 .3
                        BhvrOverride
                                    Behavior
                                    PositionOffset   0 -4 0
			End
	End


	Event
		EName 	altpivs
		Type	Local
		At	infrontadj
		Altpiv	1
		#Bhvr	Behaviors/SRFFScale.bhvr
		Geom	SkyRaiderFF
		
	End

	Event
		Type	Local
		At	altpivs
		Altpiv	1
		Part1	:Smokelight.part

	End
	
	Event
		Type	Local
		At	altpivs
		Altpiv	2
		Part1	:Smokelight.part
		
	End

	Event
		Type	Local
		At	altpivs
		Altpiv	3
		
		Part1	EnemyFx/5thColumn/HoverBot/ffThrusterGlowBlueBack.part
		Part2	EnemyFx/5thColumn/HoverBot/ffThrusterCenterBlueBack.part
		Part3	EnemyFx/5thColumn/HoverBot/ffThrustercoreBack.part
		Part4	EnemyFx/5thColumn/HoverBot/ffThrusterTrailBack.part
		Part5	EnemyFx/5thColumn/HoverBot/ffThrusterGasesBack.part

		
	End

	Event
		Type	Local
		At	altpivs
		Altpiv	4
		
		Part1	EnemyFx/5thColumn/HoverBot/ffThrusterGlowBlueFront.part
		Part2	EnemyFx/5thColumn/HoverBot/ffThrusterCenterBlueFront.part
		Part3	EnemyFx/5thColumn/HoverBot/ffThrustercoreFront.part
		Part4	EnemyFx/5thColumn/HoverBot/ffThrusterTrailFront.part
		Part5	EnemyFx/5thColumn/HoverBot/ffThrusterGasesFront.part
		
	End

	Event
		Type	Local
		At	altpivs
		Altpiv	5
		
		Part1	EnemyFx/5thColumn/HoverBot/ffThrusterGlowBlueLeft.part
		Part2	EnemyFx/5thColumn/HoverBot/ffThrusterCenterBlueLeft.part
		Part3	EnemyFx/5thColumn/HoverBot/ffThrustercoreLeft.part
		Part4	EnemyFx/5thColumn/HoverBot/ffThrusterTrailLeft.part
		Part5	EnemyFx/5thColumn/HoverBot/ffThrusterGasesLeft.part

		#Bhvr	Behaviors/powers/forcefield/ForceBubbless.bhvr
		
	End

	Event
		Type	Local
		At	altpivs
		Altpiv	6
		
		Part1	EnemyFx/5thColumn/HoverBot/ffThrusterGlowBlueRight.part
		Part2	EnemyFx/5thColumn/HoverBot/ffThrusterCenterBlueRight.part
		Part3	EnemyFx/5thColumn/HoverBot/ffThrustercoreRight.part
		Part4	EnemyFx/5thColumn/HoverBot/ffThrusterTrailRight.part
		Part5	EnemyFx/5thColumn/HoverBot/ffThrusterGasesRight.part

		#Bhvr	Behaviors/powers/forcefield/ForceBubbless.bhvr
		
	End

	Event
		Type	Local
		At	altpivs
		Altpiv	7
		
		Bhvr	Behaviors/powers/forcefield/ForceBubbless.bhvr
		Part	:ForceFieldGlow.part
		Part	:ForceFieldRing.part
		Part	:ForceFieldEdge.part
		#LifeSpan	30
	End

	Event
		EName 	bubbleGeom
		Type	Local
		At	altpivs
		Altpiv	7
		Bhvr 	behaviors/powers/forcefield/ssForceBubble3.bhvr
		
		Geom	FX_ForceBubble
		LifeSpan	50		
	End

End

Condition
	On 	cycle
	Time 	40
	Chance	7

	Event
		EName 	bubbleGeom
		Type	Local
		At	altpivs
		Altpiv	7
		Bhvr 	behaviors/powers/forcefield/ssForceBubble3.bhvr
		
		Geom	FX_ForceBubble
		LifeSpan	70		
	End

End


End
##################################

