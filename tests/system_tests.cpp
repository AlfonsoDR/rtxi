/*
 	 The Real-Time eXperiment Interface (RTXI)
	 Copyright (C) 2011 Georgia Institute of Technology, University of Utah, Weill Cornell Medical College

	 This program is free software: you can redistribute it and/or modify
	 it under the terms of the GNU General Public License as published by
	 the Free Software Foundation, either version 3 of the License, or
	 (at your option) any later version.

	 This program is distributed in the hope that it will be useful,
	 but WITHOUT ANY WARRANTY; without even the implied warranty of
	 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	 GNU General Public License for more details.

	 You should have received a copy of the GNU General Public License
	 along with this program.  If not, see <http://www.gnu.org/licenses/>.

 */

#include <system_tests.h>
#include <gmock/gmock.h>
#include <rt.h>
#include <event.h>

TEST_F(SystemTest, getInstance)
{
    system = RT::System::getInstance();
    EXPECT_EQ(system, RT::System::getInstance());
    EXPECT_EQ(system, system->getInstance());
}

TEST_F(SystemTest, getPeriod)
{
    // Check with default period
    auto period = 1000000ll;
    ASSERT_EQ(period, system->getPeriod());
}

TEST_F(SystemTest, setPeriod)
{
    auto period = 1000000ll;
    int retval = system->setPeriod(period);
    ASSERT_EQ(retval, 0);
    EXPECT_EQ(period, system->getPeriod());
    period += period;
    retval = system->setPeriod(period);
    EXPECT_EQ(period, system->getPeriod());
}

TEST_F(SystemTest, postEvent)
{
    MockRTEvent event;
    EXPECT_CALL(event, callback()).Times(::testing::AtLeast(1));
    auto retval = system->postEvent(&event, true);
    EXPECT_EQ(retval, 0);
}
