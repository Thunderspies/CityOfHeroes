-- Header file required to use Scripted Zone Events.
LoadLuaFile("Lua/SZE/SZEMain.lua")
-- Required to run Scripted Zone Event
tick = function() szeTick() end

-- This event is used in scripts/v_contacts/StMartial/SL6_7_SM_i24_Arc_Three Mission 4.
-- The point of this is to simulate a fight between Penelope Mayhem and Penelope Yin, with Mirror Spirit joining in.

-- This counter is used when you want to have the UI count up something.
countGroupHeal = function(Status, increase, timerName)
	if SZEMain.timers[timerName]:complete() then
		Status = Status + increase
		SZEMain.timers[timerName]:init()
	end
	return Status
end

-- This counter is used when you want to have an enemy health be displayed instead. It's neat!
countGroupDamage = function(Status, damage, timerName)
	if SZEMain.timers[timerName]:complete() then
		Status = Status - damage
		SZEMain.timers[timerName]:init()
	end
	return Status
end

stage1 = Stage:new{ name = "Startup" }
stage1.startup = function(self)
	Message.DebugPrint("Bringing up the UI.")

-- These timers handle Penelope Yin. The first is her regular damage. The second is when she's getting hurt even more.
-- The third is when Mirror Spirit arrives to heal her.
	SZEMain:addTimer("YinDamageTimer", Timer:new{ duration = 2 }, true)
	SZEMain:addTimer("ExtraDamageTimer", Timer:new{ duration = 2 }, true)
	SZEMain:addTimer("HealingTimer", Timer:new{ duration = 1 }, true)

--  This is Mirror Spirit's damage timer.
	SZEMain:addTimer("MirrorSpiritDamageTimer", Timer:new{ duration = 5 }, true)

--  This is Penelope Mayhem's damage timer. The second is when she's getting hurt even more by Mirror Spirit arriving.

	SZEMain:addTimer("MayhemDamageTimer", Timer:new{ duration = 3 }, true)
	SZEMain:addTimer("MayhemExtraDamageTimer", Timer:new{ duration = 2 }, true)


	if not self.UI then
		self.UI = UICollections:new
		{
			collections =
			{
				UICollection:new
				{
					-- "Stage1" refers to the UICollection from Zone Events.
					name = "Stage1",
					-- ScriptDisplayName is DisplayName that shows up on top of the UI.
					scriptDisplayName = "Penelope's Mayhem",
					-- StagedisplayName is the text that shows up inside of the actual UI.
					stageDisplayName = "Faultline Showdown",
					items =
					{
						UIItem:new
						{
							name = "Group1Text",
							uiType = "Text",
							text = "The Penelope's are fighting each other.",
						} ,
						UIItem:new
						{
							name = "Group1Bar",
							uiType = "BarRed",
							text = "Penelope Yin's Health:",
							currentVal = function() return PenelopeYinHealth end,
							targetVal = function() return 100 end
						} ,
						UIItem:new
						{
							name = "MayhemDamageTimerBar",
							uiType = "BarRed",
							text = "Penelope Mayhem's Health:",
							currentVal = function() return PenelopeMayhemHealth end,
							targetVal = function() return 100 end
						} ,
						-- Mirror Spirit's UI will not show up until Penelope Yin's status hits 40.
						UIItem:new
						{
							name = "MirrorSpiritDamageBar",
							uiType = "BarRed",
							text = "Mirror Spirit's Health:",
							currentVal = function() return MirrorSpiritHealth end,
							targetVal = function() return 100 end,
							display = function() return Mission.ObjectiveIsSucceeded("ScriptMirrorSpiritArrives") end
						} ,
					}
				}
			}
		}
	end
end

-- This says that the following should be refreshed at every tick.
stage1.tick = function(self)

-- This shows the function that's effecting each status.
-- PenelopeYinHealth is effected by CountGroupDamage twice. The first is for her normal damage. The second is for her accelerated damage.
	PenelopeYinHealth = countGroupDamage(PenelopeYinHealth or 100, 1, "YinDamageTimer")
	PenelopeYinHealth = countGroupDamage(PenelopeYinHealth or 100, 1, "ExtraDamageTimer")

-- This timer, when active, heals Penelope Yin, given that the function countGroupHeal is set to add and not subtract.
	PenelopeYinHealth = countGroupHeal(PenelopeYinHealth or 100, 2, "HealingTimer")

-- PenelopeMayhemHealth is effected by CountGroupDamage twice. The first is for her normal damage. The second is for her accelerated damage.
	PenelopeMayhemHealth = countGroupDamage(PenelopeMayhemHealth or 100, 1, "MayhemDamageTimer")
	PenelopeMayhemHealth = countGroupDamage(PenelopeMayhemHealth or 100, 4, "MayhemExtraDamageTimer")

-- This is Mirror Spirit's regular health.
	MirrorSpiritHealth = countGroupDamage(MirrorSpiritHealth or 100, 1, "MirrorSpiritDamageTimer")


-- This sets up the timers that are extra to not be started right away.
	if not self.StatusChecked and PenelopeYinHealth == 100 then
			self.StatusChecked = true
			SZEMain.timers["ExtraDamageTimer"]:stop()
			SZEMain.timers["MayhemExtraDamageTimer"]:stop()
			SZEMain.timers["HealingTimer"]:stop()
			SZEMain.timers["MirrorSpiritDamageTimer"]:stop()
	end

	if not self.StatusChecked2 and PenelopeYinHealth <= 90 then
			self.StatusChecked2 = true
			Message.DebugPrint("PenelopeYinHealth passed 90.")
			Message.Caption(ALL_PLAYERS, "<t=8 caption x=0.3 y=0.3 d=8><scale 1.75><color White><bgcolor DarkRed>Who are you?! What happened to... the other me?<t=8 caption x=0.7 y=0.3 d=8><scale 1.75><color White><bgcolor DarkOrchid>We are Penelope Mayhem, and we are here to destroy you!")
	end

	if not self.StatusChecked3 and PenelopeYinHealth <= 80 then
			self.StatusChecked3 = true
			Message.DebugPrint("PenelopeYinHealth passed 80.")
			Message.Caption(ALL_PLAYERS, "<t=8 caption x=0.3 y=0.3 d=8><scale 1.75><color White><bgcolor DarkRed>Is that all you want? Destruction?!<t=8 caption x=0.7 y=0.3 d=8><scale 1.75><color White><bgcolor DarkOrchid>To spread our hatred... our anger... to spread our control over all... over the ones who did nothing while we suffered!")
	end

-- This starts the extra damage timer on Penelope Yin.
	if not self.StatusChecked4 and PenelopeYinHealth <= 70 then
			self.StatusChecked4 = true
			Message.DebugPrint("PenelopeYinHealth passed 70.")
			Message.Caption(ALL_PLAYERS, "<t=8 caption x=0.3 y=0.3 d=8><scale 1.75><color White><bgcolor DarkRed>You don't have to do this! You can... you can resist the anger!<t=8 caption x=0.7 y=0.3 d=8><scale 1.75><color White><bgcolor DarkOrchid>There is nothing to resist. Our anger is our power. We will show the world what we can do.")
			SZEMain.timers["ExtraDamageTimer"]:start()
	end

-- This STOPS the extra damage timer on Penelope Yin.
-- This starts the extra damage timer on Penelope Mayhem.
-- This starts Mirror Spirit's damage timer.
-- This starts Penelope Mayhem's extra damage timer.
-- The objective set here displays Mirror Spirit's UI.
	if not self.StatusChecked5 and PenelopeYinHealth <= 40 then
			self.StatusChecked5 = true
			Message.DebugPrint("PenelopeYinHealth passed 30.")
			Mission.SetObjectiveComplete(1, "ScriptMirrorSpiritArrives")
			Message.Caption(ALL_PLAYERS, "<t=5 caption x=0.3 y=0.3 d=5><scale 1.75><color White><bgcolor Brown>Penelope, hold on, I will help you!<t=5 caption x=0.3 y=0.3 d=5><scale 1.75><color White><bgcolor DarkRed>Mirror Spirit!")
			SZEMain.timers["HealingTimer"]:start()
			SZEMain.timers["MirrorSpiritDamageTimer"]:start()
			SZEMain.timers["ExtraDamageTimer"]:stop()
			SZEMain.timers["MayhemExtraDamageTimer"]:start()
	end

-- This stops the event once Penelope Mayhem is at 20.
	if not self.StatusChecked6 and PenelopeMayhemHealth <= 20 then
			self.StatusChecked6 = true
			Message.DebugPrint("PenelopeMayhemHealth passed 20.")
			Mission.SetObjectiveComplete(1, "ScriptMayhemDown")
			Message.Caption(ALL_PLAYERS, "<t=5 caption x=0.5 y=0.5 d=5><scale 1.75><color White><bgcolor DarkOrchid>We can't... we won't lose... {HeroName}... will save us...!")
			self.nextStage = "Stop"
	end

end

-- Stop Stage: ----------------------------------------------------------------
stopStage = Stage:new{ name = "Stop" }
stopStage.startup = function(self)
	Message.DebugPrint("Starting Stage Stop")
	EndScript()
end

-- Name the Event
SZEMain.eventName = "StMartial_Arc_3_Mission_4"

-- Add Stages listed above
SZEMain:addStage("startup", stage1)
SZEMain:addStage("Stop", stopStage)

-- Define stage to start
SZEMain:gotoStage("startup")
