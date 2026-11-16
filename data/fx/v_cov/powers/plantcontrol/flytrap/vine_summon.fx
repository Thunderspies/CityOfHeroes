#########################################################
##	template

FxInfo
LifeSpan	150

Input
	Inpname chest
End

#######################  sounds #########################




Condition
	On	Time
	Time	25


	Event
		ename	dirtholeR
		Type 	local
		At	root
		Part1	:summon_dirt.part
		bhvr	:summon_dirtR.bhvr
		lifespan	44
	End

	Event
		ename	dirtholeL
		Type 	local
		At	root
		Part1	:summon_dirt.part
		bhvr	:summon_dirtL.bhvr
		lifespan	44
	End


End



Condition
	On	Time
	Time	45


	Event
		Type 	local
		At	C_Hind_LlegR
		part1	:summon_squirt.part
		part1	:summon_fumes.part
		bhvr	:summon_tentoff.bhvr
		lifespan 80
	End

	Event
		Type 	local
		At	C_Hind_LlegL
		part1	:summon_squirt.part
		part1	:summon_fumes.part
		bhvr	:summon_tentoff.bhvr
		lifespan 080
	End


	Event
		Type 	local
		At	dirtholeR
		part1	:summon_squirt.part
	end

	Event
		Type 	local
		At	dirtholeL
		part1	:summon_squirt.part
	end

End


End

	

	