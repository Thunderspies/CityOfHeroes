#########################################################
##	onfire
##
FxInfo  

Input  
	InpName	hips
End

Input  
	Inpname	chest
End

Input  
	Inpname	root
End

LifeSpan	100




Condition
	On 	Time
	Time 	0

#	Event
#		EName 	Hook
#		Type	Local 
#		At	root
#		Geom	RootToChestAdjustment
#		#Sound	Atomicblast 100 30 .9
#
#	End

	Event
		EName 	Fire
		Type	Posit
		At	hips
		Part2	:BS_Blasthit_Burn.part ## small cont fire
		Part3	:BS_Blasthit_embers.part ## embers
		Sound elecexplo2 80.0 80.0 .7
	End


	Event
		EName 	Prime1
		Type	Local 
		At	chest
		
		Part1	:BS_Blasthit_Core.part ## hot white splashy
		Part2	:BS_Blasthit_Explosion.part ##fireball
		Sound bs_hit3 80.0 80.0 .99
		Sound bs_hit2 80.0 80.0 .99
		Sound bs_hit1 80.0 80.0 .99
		Sound bs_hit3 80.0 80.0 .99



	End

		Event
		Type	Local 
		At	chest

		Part1	:BS_Tail_BallTailBig.part
		Part1	:BS_Tail_ShotBallGlow.part
		Part1	:BS_Tail_SteamTrail.part
		part1	:BS_Tail_ShrapnelSmoke.part
		part1	:BS_Tail_ShrapnelSmoke.part
		part1	:BS_Tail_ShrapnelSmoke.part
		bhvroverride
			PyrRotate	-90 0 0
			scale		.1 .1 .1
		end

	End



End

Condition
	On 	Time
	Time	9

	Event
		EName 	Prime1
		Type	Destroy
	End

End

Condition
	On 	Time
	Time	3

	Event
		EName 	Prime2
		Type	Destroy
	End

End


Condition
	On 	Time
	Time	7

	Event
		EName 	Prime
		Type	Destroy
	End

End	

Condition
	On 	Time
	Time 	30

	Event
		EName 	RadiusFxScale
		Type	Destroy 
	End


End

Condition
	On 	Time
	Time 	120

	Event
		EName 	All
		Type	Destroy 
	End


End


End			
